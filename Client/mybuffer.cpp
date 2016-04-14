#include "mybuffer.h"
bool newCirc = true;
SOCKET mySocket;
circlebuff cData;

myBuffer::myBuffer()
{
    fp = fopen("testout", "wb");
    QAudioFormat format;
    realPos = 0;
    format.setSampleRate(44100); // Usually this is specified through an UI option
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    player = new QAudioOutput(format, this);
    if(cData.alive)
        cData.clear();
    else
        cData.init();
    curSong = 0;
    filler.resize(BUFFSIZE);
    loader = buff;
    currentTail = 0;

    this->open(QIODevice::ReadOnly);
}

void myBuffer::setSlider(){

    float percent =  300 * currentPos / (float)songTotal;
    QMetaObject::invokeMethod(mw, "updateSlider",Qt::QueuedConnection, Q_ARG(float,percent), Q_ARG(int, songTime));
}
void myBuffer::rewind(){

    int progress = cData.headBuff - currentTail;

    if(progress > 495){
        cData.tail = cData.headBuff - 495;
        currentPos = songStart + progress - 495;

    }
    else{
        cData.tail = currentTail;
        currentPos = songStart;
    }

}

qint64 myBuffer::readData(char * data, qint64 len){

    int endSong;
    if(newCirc){
       setSlider();

        if(!(endSong = cData.peak(loader, curSong))){
            return 0;
        }
        if(endSong == -1){
            curSong = loader[0];
            char yaok[40];
            memcpy(yaok, loader, 40);
            setHeader(yaok);
            return 0;
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

void myBuffer::pausePlayer(){
    player->suspend();
}

void myBuffer::resumePlayer(){
    if(player->state() == QAudio::IdleState){
        player->start(this);
        cData.tail = cData.headBuff;
    }
    player->resume();

}

void myBuffer::updateVolume(float v){
    player->setVolume(v);
}

void myBuffer::jumpLive(){
    cData.tail = cData.headBuff;
    currentPos = songStart + cData.headBuff - currentTail;
}
void myBuffer::sliderChange(int perc){
    int reqBlock = perc / 300.0f * songTotal;
    int minBlock, maxBlock;
    minBlock = songStart;
    maxBlock = songStart + cData.headBuff - currentTail;
    if(cData.headBuff > (currentTail + 498)){
        int totalProg = cData.headBuff - currentTail - 495;
        minBlock = songStart + totalProg;
        if(reqBlock < minBlock){
            cData.tail = currentTail + totalProg;
            currentPos = songStart + totalProg;
            return;
        }
    }
    if(reqBlock > maxBlock){
        cData.tail = cData.headBuff;
        currentPos = maxBlock;
        return;
    }
    if(reqBlock < minBlock){
        cData.tail = currentTail;
        currentPos = minBlock;
        return;
    }
    int diff = reqBlock - songStart;
    cData.tail = currentTail + diff;
    currentPos = songStart + diff;
    /*
    int newPos = currentTail + (perc * songTotal / 100);
    float ret;
    if(newPos > cData.head){
        cData.tail = cData.headBuff;
        currentPos = songStart + cData.headBuff - currentTail;
    }else{
        if((newPos + 475) < cData.headBuff){
            newPos = cData.headBuff - 475;
        }
        if(newPos < 0)
            newPos = 0;
        cData.tail = newPos;
        currentPos = songStart + cData.tail - currentTail;
    }*/
    //printf("Head: %d, Tail: %d", cData.head, cData.tail);
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

    /*while(1){
        if(cData.tail < cData.headBuff){
        startPlayer();
        break;
        }
    }
    cData.tail = cData.headBuff;*/
    startPlayer();
}

void myBuffer::setHeader(char * h){
    QString orig(h);
    QAudioFormat format;
    QStringList ls = orig.split("-");
    int songNumber, ss, samp, chan;
    //songNumber = ls.value(0).toInt();
    ss = ls.value(1).toInt();
    samp = ls.value(2).toInt();
    chan = ls.value(3).toInt();
    songTotal = ls.value(4).toInt();
    songNumber = ls.value(5).toInt();
    currentPos = ls.value(6).toInt();

    songTime = (songTotal * BUFFSIZE) / (samp * ss * chan / 8);

    currentTail = cData.tail;
    songStart = currentPos;
    realPos = 0;
    format.setSampleRate(samp); // Usually this is specified through an UI option
    format.setChannelCount(chan);
    format.setSampleSize(ss);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    player->stop();
    player = new QAudioOutput(format, this);


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
    player->start(this);
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
