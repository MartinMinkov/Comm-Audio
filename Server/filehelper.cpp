#include "filehelper.h"

using namespace std;
DWORD WINAPI readStuff(LPVOID param);
circlebuff c;
HANDLE newData;
HANDLE readDone;
HANDLE fileDone;
filehelper::filehelper()
{
    LPCWSTR ok1 = L"hi";
    LPCWSTR ok2 = L"IDK";
    LPCWSTR ok3 = L"WHY";
    newData = CreateEvent(NULL, TRUE, FALSE, ok1);
    readDone = CreateEvent(NULL, TRUE, FALSE, ok2);
    fileDone = CreateEvent(NULL, TRUE, FALSE, ok3);
}
void filehelper::handleDownloadRequest(QString title, SOCKET m_socket){
    networkutility::debugMessage("download requested");
    char * fileName;
    char buff[FILEMAX] = { 0 };
    fileName = buff;
    char * ff;
    qint64 left;
    networkutility n;
    int bytesRead;
    qDebug() << title;
    FILE * fqt;
    title = title.remove(0, 1);

    int index = 0;
    //find the corresponding song title with filepath
    for(QStringList::iterator it = playlist.begin(); it != playlist.end(); ++it, index++){
        QString current = *it;
        if(QString::compare(current,title,Qt::CaseSensitive) == 0){
            title = playlistWithPath.at(index);
            break;
        }
    }

    if(!(fqt = fopen(title.toStdString().c_str(), "rb+"))){
        char buff2[2];
        strcpy(buff2, ERROR_BIT);
        //char * buff2 = ERROR_BIT;
        sendDataTCP(m_socket, buff2);
        return;
    }

    char success[] = "M";
    sendDataTCP(m_socket, success);
    while((bytesRead = fread(buff, sizeof(char), FILEMAX, fqt))){
            if(bytesRead != FILEMAX){
            n.WSAS(m_socket, buff, bytesRead, 1000);
            break;
        }
        n.WSAS(m_socket, buff, 60000, 1000);
    }
}

void filehelper::handleUploadRequest(QString songName, SOCKET m_socket){
    networkutility::debugMessage("upload requested");
    networkutility n;
    HANDLE writeThread;
    DWORD id;
    int len;
    FILE * fqt;
//    songName = "vtest.wav";
    char readBuff[FILEMAX];
    c.init();
    songName = songName.remove(0, 1);
    char retBuff[1024];
    char buff[FILEMAX];
    if(!(fqt = fopen(songName.toStdString().c_str(), "wb"))){
        strcpy(retBuff, ERROR_BIT);
        n.WSAS(m_socket, retBuff, 1024, 2000);
    }
    else{
        retBuff[0] = 'M';
        n.WSAS(m_socket, retBuff, 1024, 2000);
    }
    writeThread = CreateThread(NULL, 0, readStuff, (void *)fqt, 0 , &id);
    while((len = WSARead(m_socket, readBuff, 2000, FILEMAX))){

        c.push(readBuff, len);
        SetEvent(newData);
    }
    SetEvent(readDone);
    WaitForSingleObject(fileDone, FILEMAX);
    printf("Done reading");
    fflush(stdout);

}
DWORD WINAPI readStuff(LPVOID param){
    //char * title = (char *)param;
    FILE * fqt = (FILE *)param;
    DWORD err;
    int len;
    char readBuff[FILEMAX] = { 0 };
    HANDLE events[2];
    events[0] = newData;
    events[1] = readDone;
    char * wrt;
    //fqt = fopen(title, "wb");
    wrt = readBuff;
    while(1){
        err = WaitForMultipleObjects(2, events, FALSE, INFINITE);
        ResetEvent(newData);
        while(len = c.pop(wrt)){
            fwrite(wrt, sizeof(char), len, fqt);
            printf("head %d tail %d, length: %d\n", c.head, c.tail, len);
            memset(readBuff, '\0', FILEMAX);
        }
        if(err == WAIT_OBJECT_0 + 1){
            SetEvent(fileDone);
            fclose(fqt);
            break;
        }

    }
    return 0;
}
