#include "mybuffer.h"

int totalRet = 0;
bool newCirc = true;
FILE * curSong;
SOCKET my_socket;
char * fileLoader;
HANDLE needData;
bool needD = true;
bool needNew = true;
char fileBuff[MAXLEN];
networkutility n;
SOCKET m_socket;
HANDLE sync;
myBuffer::myBuffer(int sock)
{   m_socket = sock;
    DWORD err = GetLastError();
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
 //   SetEvent(needData);
    //loadSong();
    printf("CONSTRUCTOR CALLED");
    fflush(stdout);
    this->open(QIODevice::ReadOnly);
  //  player->start(this);
    player->setVolume(0.0);
}
void myBuffer::getSong(char * songName){
    FILE * fqt;
    fqt = fopen("runescape.wav", "rb");
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
        if(cData.tail > cData.buffHead){
            SetEvent(needData);
            if(cData.head == 0){
                return 0;
            }
        }
        if(!(endSong = cData.pop(loader))){
            printf("End of song/buffer");
            return -1;
        }
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
    }
}
void myBuffer::setSocket(int socket){
    my_socket = socket;
}

bool myBuffer::loadSong(){
    printf("Loading data");
    fflush(stdout);
    if(needNew){
        needNew = false;
        if(!(curSong = fopen("runescape.wav", "rb")))
            exit(1);
    }
    int len;
    for(int i = 0; i < 10; i++){
        printf("Pushing song");
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
    SetEvent(sync);
    return true;
}

DWORD WINAPI readFromFile(LPVOID param){
    myBuffer * f = (myBuffer *)param;
    DWORD err;
    bool needASong;
    while(1){
        err = WaitForSingleObject(needData, 1000000);
       // if(needD){
        err = GetLastError();
        needASong = f->loadSong();
        err =  ResetEvent(needData);
        err = GetLastError();

        if(needASong){
            //SetEvent(loadUpSong);
        }

    }

}

qint64 myBuffer::writeData(const char *data, qint64 len){
    return 0;
}

qint64 myBuffer::bytesAvailable(){
    return 0;
}

void myBuffer::startPlayer(){

    DWORD id;
    needData =  CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)L"needData" );
    sync =  CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)L"sync" );
    fileReader = CreateThread(NULL, 0, readFromFile, (void *)this, 0, &id);
    SetEvent(needData);
  // Sleep(50);
    WaitForSingleObject(sync, 1000000);
    player->start(this);
}
