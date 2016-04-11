#include "mybuffer.h"
int totalRet = 0;
bool newCirc = true;
SOCKET mySocket;
HANDLE fillBuff;
char fillerC[BUFFSIZE] = { 0 };
circlebuff cData;
QObject * bf;
char * fillerP;
myBuffer::myBuffer()
{
    bf = this;
    QAudioFormat format;
    realPos = 0;
    format.setSampleRate(16100); // Usually this is specified through an UI option
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    player = new QAudioOutput(format, this);
    cData.init();
    curSong = 0;
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
void myBuffer::setSlider(){
    float percent =  100 * currentPos / (float)songTotal;
    //printf("Song progress: %f \n", percent);
    QMetaObject::invokeMethod(mw, "updateSlider",Qt::QueuedConnection, Q_ARG(int, (int)percent));
}

qint64 myBuffer::readData(char * data, qint64 len){

    fflush(stdout);
    int endSong;
    if(newCirc){

        setSlider();
        if(!(endSong = cData.peak(loader, curSong))){
            printf("End of song/buffer");
            return -1;
        }
        if(endSong == -1){
            curSong = loader[0];
            char yaok[40];
            memcpy(yaok, loader, 40);
            setHeader(yaok);
            return -1;
        }
        currentPos++;

        newCirc = false;
        realPos = 40;
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
void myBuffer::jumpLive(){
    cData.tail = cData.headBuff;
    currentPos = cData.headBuff - currentTail;
}
void myBuffer::sliderChange(int perc){

    printf("slider changing");
    fflush(stdout);
    int newPos = currentTail + (perc * songTotal / 100);
    float ret;
    if(newPos > cData.head){
        cData.tail = cData.headBuff;
        currentPos = songStart + cData.headBuff - currentTail;
    }else{
        if((newPos + 475) < cData.headBuff){
            printf("Doing things");
            fflush(stdout);
            newPos = cData.headBuff - 475;
        }
        cData.tail = newPos;
        currentPos = songStart + cData.tail - currentTail;
    }
    printf("Head: %d, Tail: %d", cData.head, cData.tail);
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
        //if(cData.tail < cData.headBuff){
        //Sleep(1000);
        Sleep(500);
        startPlayer();
        printf("Startng the player");
         break;
        //}

    }
}
void myBuffer::setHeader(char * h){
    QString orig(h);
    QAudioFormat format;
    QStringList ls = orig.split("-");
    int songNumber, ss, samp, chan;
//    songNumber = ls.value(0).toInt();
    ss = ls.value(1).toInt();
    samp = ls.value(2).toInt();
    chan = ls.value(3).toInt();
    songTotal = ls.value(4).toInt();
    songNumber = ls.value(5).toInt();
    currentPos = ls.value(6).toInt();
    currentTail = cData.tail;
    songStart = currentPos;
    printf("Format Stuff: %d %d %d", ss, samp, chan);
    realPos = 0;
    format.setSampleRate(samp); // Usually this is specified through an UI option
    format.setChannelCount(chan);
    format.setSampleSize(ss);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    player->stop();
    player = new QAudioOutput(format, this);
    printf("CHANGING PLAYER");
    fflush(stdout);

    //update the currently playing label based on the song index
    //minus 1 to songNumber because for some reason it starts at 1 and not 0 on the server side
    QString currentSongName;
    if(songNumber > playlist.size()-1){
        //the songindex and playlist are out of sync, print unavailable to currently playing text
        currentSongName = "unavailable";
    } else {
        currentSongName = playlist.at(songNumber);
    }
    emit updateCurrentlyPlaying(currentSongName);
}

qint64 myBuffer::writeData(const char *data, qint64 len){
    return 0;
}
qint64 myBuffer::bytesAvailable(){
    return 0;
}

void myBuffer::startPlayer(){
    player->start(this);
}
