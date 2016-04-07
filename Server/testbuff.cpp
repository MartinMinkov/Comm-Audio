#include "testbuff.h"
#include "globals.h"
#include "streamhandlerthread.h"

QAudioFormat qft;
int pt;
bool nextSong = false;
bool NN = true;
int total;
testBuff::testBuff(QString songName)
{
    loader = buff;
    fqt.setFileName("ec1.wav");
    if(!(fqt.open(QIODevice::ReadOnly))){
            exit(1);
}

    this->open(QIODevice::ReadOnly);
}
qint64 testBuff::readData(char * data, qint64 len){
    int length;
    if(NN){
        if((length = fqt.read(loader, BUFFSIZE)) != BUFFSIZE){
            if(length == 0){
                //emit functionNamehere();

                return -1;
            }
            else{
               nextSong = true;
            }
        }
        total = 0;
        NN = false;
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
        memcpy(data, loader, len);
        loader = &buff[0];
        total += len;
        return len;


    memcpy(data, loader, length);


    return length;

    /*int endSong;
    if(newCirc){
        if(cData.tail > cData.buffHead){
            printf("Requesting more data");
            SetEvent(needData);
            if(cData.head == 0){
                printf("Returning 0 ok");
                fflush(stdout);

                return 0;
            }
            //needD = true;

        }
        if(!(endSong = cData.pop(loader))){
            printf("End of song/buffer");
            return -1;
        }
        //sendDataTCP(my_socket, loader);
       // n.WSAS(my_socket, loader, BUFFSIZE, 10000);
        sendToMultiCast(loader);
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
    }*/
}
}
qint64 testBuff::writeData(const char * data, qint64 len){
 return -1;
}

qint64 testBuff::bytesAvailable(){
    return 0;
}
