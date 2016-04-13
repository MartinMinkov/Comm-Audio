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
    packetNum = 0;
    headerLength = 40;

    //get the total song from playlistWithPath
    totalSong = playlistWithPath.size();
    songNumber = 1;
    currentSong = 0;

    this->open(QIODevice::ReadOnly);
    loadSong();


   // qbt = fqt.readAll();
   // fqt.close();
}
int testBuff::checkWinner(){
    int max = 0;
    int pos = -1;
    for(int i = 0; i < SLISTSIZE; i++){
        if(songVoting[i] > max){
            max = songVoting[i];
            pos = i;
        }
    }
    printf("Vote for song #: %d", pos);
    return pos;
}
void testBuff::zeroSongList(){
    for(int i = 0; i < SLISTSIZE; i++){
        songVoting[i] = 0;
    }
}

bool testBuff::loadSong(){
    printf("in constructor");
            fflush(stdout);
    nextSong = false;
    QString songNameWithPath;
    QString songName;
    int posNext = checkWinner();
    if(posNext > -1){
        songNameWithPath = playlistWithPath.at(posNext);
        songName = playlist.at(posNext);
        theCurrent = posNext;
    }else{
    songNameWithPath = playlistWithPath.at(currentSong % totalSong);
    songName = playlist.at(currentSong % totalSong);
    theCurrent = currentSong % totalSong;
    }
    zeroSongList();
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

    return true;
}
void testBuff::pauseStream(){
    player->suspend();
}
void testBuff::resumeStream(){
    player->resume();
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
    hold += "-" + QString::number(theCurrent % playlist.size()) + "-";
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
       return true;
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
    }else{
        loader += total;
        memcpy(data, loader + 40, len);
        loader = &buff[0];
        total += len;
        return len;
    }
    //end brace was after return length instead?
    memcpy(data, loader, length);

    return length;
}

qint64 testBuff::writeData(const char * data, qint64 len){
    return -1;
}

qint64 testBuff::bytesAvailable(){
    return 0;
}
