#include "mybuffer.h"
#include "networkutility.h"
#include "globals.h"
#define MAXLEN 60000
int totalRet = 0;
bool newCirc = true;
FILE * curSong;
char * fileLoader;
HANDLE needData;
bool needNew = true;
char fileBuff[MAXLEN];
SOCKET m_socket;
myBuffer::myBuffer(int sock)
{   m_socket = sock;
    QAudioFormat format;
    realPos = 0;
    format.setSampleRate(44100); // Usually this is specified through an UI option
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    player = new QAudioOutput(format, this);
    cData.init();
    filler.resize(BUFFSIZE);
    loader = buff;
    DWORD id;
    fileLoader = fileBuff;
    //getSong("stress.wav");
    fileReader = CreateThread(NULL, 0, readFromFile, (void *)this, 0, &id);
    SetEvent(needData);
    //loadSong();
    printf("CONSTRUCTOR CALLED");
    fflush(stdout);
    this->open(QIODevice::ReadOnly);
    player->start(this);
}
void myBuffer::getSong(char * songName){
    FILE * fqt;
    fqt = fopen("stress.wav", "rb");
   // fqt = fopen("warpeace.txt", "rb");
    char arrBuff[BUFFSIZE] = { 0 };
    char * ok = arrBuff;
    int len;
    int count = 0;
    while((len = fread(ok, sizeof(char), BUFFSIZE, fqt))){
        cData.push(ok, len);
        if(count > 500)
            break;
    }

}
qint64 myBuffer::readData(char * data, qint64 len){
    int endSong;
    if(newCirc){
        if(cData.tail <= cData.buffHead){
            SetEvent(needData);
        }
        if(!(endSong = cData.pop(loader))){
            printf("End of song/buffer");
            return -1;
        }
        sendDataTCP(m_socket, loader);
        newCirc = false;
    }

    int remain = BUFFSIZE - realPos;
    if(remain < len){
        loader += realPos;
        memcpy(data, loader, remain);
        loader = &buff[0];
        memset(buff, '\0', BUFFSIZE);
        newCirc = true;
        realPos = 0;
        return remain;
    }
    else{
        loader += realPos;
        memcpy(data, loader, len);
        loader = &buff[0];
        realPos += len;
        return len;
    }
}
bool myBuffer::loadSong(){
    if(needNew){
        needNew = false;
        if(!(curSong = fopen("stress.wav", "rb")))
            exit(1);
    }
    int len;
    for(int i = 0; i < 10; i++){
        len = fread(fileLoader, sizeof(char), MAXLEN, curSong);
        if(!len){
            needNew = true;
            return false;
        }
        cData.push(fileLoader, len);
        if(len < MAXLEN){
            needNew = true;
            return false;
        }
    }
    return true;
}

DWORD WINAPI readFromFile(LPVOID param){
    myBuffer * f = (myBuffer *)param;
    bool needASong;
    while(1){
        WaitForSingleObject(needData, INFINITE);
        needASong = f->loadSong();
        ResetEvent(needData);
        if(needASong){
            //SetEvent(loadUpSong);
        }
    }

}




qint64 myBuffer::writeData(const char *data, qint64 len){

}
qint64 myBuffer::bytesAvailable(){
    return 0;
}

void myBuffer::startPlayer(){
    player->start(this);
}
