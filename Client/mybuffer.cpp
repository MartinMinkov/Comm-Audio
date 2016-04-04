#include "mybuffer.h"
int totalRet = 0;
bool newCirc = true;
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
    getSong("stress.wav");
    this->open(QIODevice::ReadOnly);
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
        if(!(endSong = cData.pop(loader)))
            printf("End of song/buffer");
            newCirc = false;
        }
    //printf("Requesting : %d", len);
    fflush(stdout);

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
   /* if(realPos >= len){
        memcpy(data, loader, len);
       // fwrite(data, sizeof(char), len, testOutput);

        realPos -= len;
        totalRet += len;
        printf("%d\n", totalRet);
        return len;
    }
   else{

        int offset = BUFFSIZE - realPos;
        loader += offset;
        memcpy(data, loader, realPos);
       // fwrite(data, sizeof(char), realPos, testOutput);

        loader = &buff[0];
        int hold = realPos;
        totalRet += hold;
        printf("t: %d\n", totalRet);
        realPos = 0;
        return hold;
*/



}
qint64 myBuffer::writeData(const char *data, qint64 len){

}
qint64 myBuffer::bytesAvailable(){
    return 0;
}

void myBuffer::startPlayer(){
    player->start(this);
}
