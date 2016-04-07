#include "playermanager.h"
#include "testbuff.h"
#include "globals.h"
testBuff * t;
playerManager::playerManager()
{
    LPCWSTR tom = L"test";
    songDone =  CreateEvent(NULL, TRUE, FALSE,tom );


}
void playerManager::startSong(QString songName){
    //char header[50];
    //fqt.read(header, 50);
    std::vector<int> vect;
    WavFile wvf;
    wvf.open(songName);
    vect = wvf.getStuff();
    wvf.close();
    setFormat(vect);
}

bool playerManager::setFormat(std::vector<int> vect){
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
       QString song("ec1.wav");
       t = new testBuff(song);

       player = new QAudioOutput(format, t);
       //player->start();
        //connect(t, SIGNAL(functionName()), this, SLOT(endPlayer()));


}
void playerManager::startPlayer(){
    player->start(t);
    player->setVolume(0.0);
}
void playerManager::endPlayer(){
    printf("Ending player");
}
