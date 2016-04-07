#include "mybuffer.h"
#include "networkutility.h"
int totalRet = 0;
bool newCirc = true;
SOCKET mySocket;
HANDLE fillBuff;
char fillerC[BUFFSIZE] = { 0 };
circlebuff cData;

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
    printf("reading data %d\n", len);
    printf("Head: %d, Tail: %d\n", cData.head, cData.tail);
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
    DWORD RecvBytes = 0, Index;
    DWORD Flags = 0;
    myBuffer * player = (myBuffer *)param;
    int len;
    while(1){
        //len = WSARecvFrom(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, (struct sockaddr *)&streamServer, &server_len, &(SI->Overlapped), ServerRoutine);
        //cData.push(SI->DataBuf, len);
    }
}

void myBuffer::setSocket(int socket){
    mySocket = socket;
    DWORD id;
    //fillBuff = CreateThread(NULL, 0, fillUp, (void *)this, 0, &id);
    while(1){
        printf("Tail %d, Head: %d", cData.tail, cData.headBuff);
        fflush(stdout);
           // if(cData.tail < cData.headBuff){
        Sleep(1000);
        startPlayer();
            printf("Startng the player");
            break;
        //}

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
