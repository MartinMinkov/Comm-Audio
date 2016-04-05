#include "mybuffer.h"
#include "networkutility.h"
int totalRet = 0;
bool newCirc = true;
SOCKET mySocket;
HANDLE fillBuff;
char fillerC[BUFFSIZE] = { 0 };

char * fillerP;
myBuffer::myBuffer()
{
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
    testOutput = fopen("out.txt", "wb+");
    filler.resize(BUFFSIZE);
    loader = buff;
    //getSong("stress.wav");
    fillerP = fillerC;
    this->open(QIODevice::ReadOnly);
}
void myBuffer::getSong(char * songName){
    FILE * fqt;
    fqt = fopen("ec1.wav", "rb");
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
        if(!(endSong = cData.pop(loader))){
            printf("End of song/buffer");
            return -1;
        }
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

DWORD WINAPI fillUp(LPVOID param){
    myBuffer * player = (myBuffer *)param;
    int len;
    while(1){
        len = WSARead(mySocket, fillerP, 10000000, BUFFSIZE);
        player->cData.push(fillerP, len);
    }
}

void myBuffer::setSocket(int socket){
    mySocket = socket;
    DWORD id;
    fillBuff = CreateThread(NULL, 0, fillUp, (void *)this, 0, &id);
    while(1){
        if(cData.tail < cData.headBuff){
            startPlayer();
            break;
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