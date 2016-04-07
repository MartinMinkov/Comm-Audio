#include "playermanager.h"
#include "testbuff.h"
#include "globals.h"

testBuff * t;
playerManager::playerManager()
{
    //LPCWSTR tom = L"test";
    //songDone =  CreateEvent(NULL, TRUE, FALSE,tom );
    songNumber = 1;
    memset(headerBuff, '\0', 40);
    header = headerBuff;

}
void playerManager::startSong(QString songName){
       std::vector<int> vect;
    WavFile wvf;
    qDebug() << songName;
    wvf.open(songName);
    vect = wvf.getStuff();

    wvf.close();

    QFile qft(songName);
    qft.open(QIODevice::ReadOnly);

    fileSize = qft.size();
    qft.close();
    getHeader(vect);
    printf("Crashed here");
    fflush(stdout);
    setFormat(vect);
    printf("past all the header stuff");
    fflush(stdout);
}
void playerManager::getHeader(std::vector<int> vect){
    QString hold = "";
    int a,b,c;
    a = vect.at(0);
    b = vect.at(1);
    c = vect.at(2);
    char d = songNumber++;
    hold += d;
    hold += "-" + QString::number(a);
    hold += "-" + QString::number(b);
    hold += "-" + QString::number(c);
    hold += "-" + QString::number(fileSize/ BUFFSIZE);

    memcpy(header, hold.toStdString().c_str(), 40);
    printf(header);
    fflush(stdout);
}

bool playerManager::setFormat(std::vector<int> vect){
       QAudioFormat format;

       printf("%d %d %d", vect.at(0), vect.at(1), vect.at(2));
       fflush(stdout);
       format.setSampleRate(66666);
       format.setChannelCount(2);
       format.setSampleSize(16);
       format.setCodec("audio/pcm");
       format.setByteOrder(QAudioFormat::LittleEndian);
       format.setSampleType(QAudioFormat::UnSignedInt);
        printf("Done setting format");
        fflush(stdout);
       QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
       if (!info.isFormatSupported(format)) {
           qWarning() << "Raw audio format not supported by backend, cannot play audio.";
           return false;
       }
       QString song("ec1.wav");
       t = new testBuff(song, NULL);

       //player = new QAudioOutput(format, t);
       //player->start();
        //connect(t, SIGNAL(functionName()), this, SLOT(endPlayer()));


}
void playerManager::startPlayer(){
    //player->start(t);
    //player->setVolume(0.0);

}
void playerManager::endPlayer(){
    printf("Ending player");
}
