#include "testbuff.h"

QAudioFormat qft;
int pt;
bool nextSong = false;
bool NN = true;
int total;
testBuff::testBuff(QString songName, QAudioOutput * p)
{   player = p;
    header = headerBuffer;
    loader = buff;
   // memset(header, '\0', 40);
   // memcpy(header,heads, 40);
   // fqt.setFileName("ec1.wav");
   /* if(!(fqt.open(QIODevice::ReadOnly))){
            exit(1);
    }
    */
    packetNum = 0;
    headerLength = 40;

    //get the total song from playlistWithPath
    totalSong = playlistWithPath.size();
    songNumber = 1;
    currentSong = 0;

    //playList.push_back("my_mule.wav");
//    playList.push_back("runescape.wav");

//    playList.push_back("C:/Users/Alvin/Documents/songs/pokemon1.wav");
//    playList.push_back("C:/Users/Alvin/Documents/songs/pokemon2.wav");
//    playList.push_back("C:/Users/Alvin/Documents/songs/pokemon3.wav");
    this->open(QIODevice::ReadOnly);
    loadSong();


   // qbt = fqt.readAll();
   // fqt.close();
}
bool testBuff::loadSong(){
    printf("in constructor");
            fflush(stdout);
    nextSong = false;
    QString songNameWithPath = playlistWithPath.at(currentSong % totalSong);
    QString songName = playlist.at(currentSong % totalSong);
    currentSong++;
    fqt.setFileName(songNameWithPath);
    //set the currently playing text in server here
    emit triggerUpdateCurrentlyPlayingLabel(songName);

    if(!(fqt.open(QIODevice::ReadOnly))){
            return false;
    }

    qbt = fqt.readAll();
    fileSize = qbt.size();
    fqt.close();
    WavFile wvf;
    wvf.open(songNameWithPath);
    std::vector<int>vect = wvf.getStuff();
    wvf.close();
    getHeader(vect);
    setFormat(vect);



}
void testBuff::getHeader(std::vector<int> vect){
    QString hold = "";
    int a,b,c;
    a = vect.at(0);
    b = vect.at(1);
    c = vect.at(2);
    char d = songNumber;
    hold += d;
    hold += "-" + QString::number(a);
    hold += "-" + QString::number(b);
    hold += "-" + QString::number(c);
    hold += "-" + QString::number(fileSize/ BUFFSIZE);
    hold += "-" + QString::number((songNumber-1) % playlist.size()) + "-";
    memset(header, '\0', 40);
    memcpy(header, hold.toStdString().c_str(), 40);
    songNumber++;

    printf(header);
    headLength =  strlen(header);
    fflush(stdout);
}

bool testBuff::setFormat(std::vector<int> vect){

       QAudioFormat format;
       format.setSampleRate(vect.at(1));
       format.setChannelCount(vect.at(2));
       format.setSampleSize(vect.at(0));
       format.setCodec("audio/pcm");
       format.setByteOrder(QAudioFormat::LittleEndian);
       format.setSampleType(QAudioFormat::UnSignedInt);
       QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
       if (!info.isFormatSupported(format)) {
           qWarning() << "Raw audio format not supported by backend, cannot play audio.";
           return false;
       }
       if(player != NULL)
       player->stop();
       player = new QAudioOutput(format, this);
       player->setVolume(0.0);
       currentPos = 0;
       player->start(this);


}
qint64 testBuff::readData(char * data, qint64 len){
    int length;
    QByteArray chunk;
    fflush(stdout);
    if(NN){
        if(nextSong){
            loadSong();
            return 0;
        }
        currentPos++;
        memcpy(loader, header, 40);
        length = qbt.size();
        if(length > BUFFSIZE){
            chunk = qbt.left(BUFFSIZE - headerLength);
            qbt = qbt.remove(0, BUFFSIZE - headerLength);
            sprintf(loader + headLength, "%d", currentPos);
            memcpy(loader + 40, chunk.toStdString().c_str(), BUFFSIZE - headerLength);

        }else{
            if((len + headerLength) > BUFFSIZE){
                len = BUFFSIZE - headerLength;
            }
            chunk = qbt.left(len);
            qbt = qbt.remove(0, len);
            sprintf(loader + headLength, "%d", currentPos);
            memset(loader + 40, '\0', BUFFSIZE - 40);
            memcpy(loader + 40, chunk.toStdString().c_str(), len);
            nextSong = true;
        }
        total = 0;
        NN = false;
        fflush(stdout);
        sendToMultiCast(loader);
    }
    int remain = BUFFSIZE - total;
    if(remain < len){
        loader += total;
        memcpy(data, loader, remain);
        loader = &buff[0];
        memset(buff, '\0', BUFFSIZE);
        NN = true;
        total = 0;
        return remain;
    }
    else{
        loader += total;
        memcpy(data, loader + 40, len);
        loader = &buff[0];
        total += len;
        return len;



    memcpy(data, loader, length);

    return length;


}
}
qint64 testBuff::writeData(const char * data, qint64 len){
 return -1;
}

qint64 testBuff::bytesAvailable(){
    return 0;
}
