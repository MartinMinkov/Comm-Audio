#include "threadmanager.h"
#include <windows.h>
#include "circlebuff.h"
#include "networkutility.h"
#include <QFile>
#include <QDataStream>
#define FILEMAX 20000

HANDLE newData;
HANDLE readDone;
HANDLE fileDone;
circlebuff c;
ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{
    sd = 0;
}

ThreadManager::~ThreadManager()
{
    closesocket(sd);
    sd = 0;
    WSACleanup();
}

void ThreadManager::connect(QString ipaddr, QString portnum, QString username)
{
    newData = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)"newData");
    readDone = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)"fileDone");

    fileDone = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)"fileDo2ne");
    if(sd != 0) {
        qDebug() << "Socket not null";
        return;
    }

    if(ipaddr == "" || portnum == "") {
        qDebug() << "No IP address or port number";
        return;
    }

    if ((ret = WSAStartup(wVersionRequested, &stWSAData)) != 0)
    {
        qDebug() << "WSAStartup failed";
        return;
    }

    //TCP Socket
    if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        qDebug() << "Cannot create TCP socket";
        return;
    }

    // Initialize and set up the address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(portnum.toStdString().c_str()));
    if ((hp = gethostbyname(ipaddr.toStdString().c_str())) == NULL)
    {
        formatMessage("Unknown server address");
        return;
    }
    // Copy the server address
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    // Connectin    g to the server
    if (::connect(sd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        formatMessage("Can't connect to server");
        return;
    }

    //Get and send name
    if(username != "") {
        sendDataTCP(sd, username.toStdString().c_str());
        qDebug() << username;
    }
    else {
        qDebug() << "No name";
    }

    emit finished();
}
void ThreadManager::handleRequest()
{
    int BytesRead;
    char *bp, buf[PACKET_LEN];
    Sleep(2500);
    SendDownloadRequest();
    while (true)
    {
        int bytesToRead = PACKET_LEN;
        char *bp = buf;
        if ((BytesRead = recv(sd, bp, bytesToRead, 0)) > 0)
        {
            bytesToRead -= BytesRead;
        }

        /* recv() failed */
        if(BytesRead < 0)
        {
            qDebug() << "recv() failed";
            emit signalDisconnect();
        }
        /* client disconnected */
        if(BytesRead == 0)
        {
            qDebug() << "CLIENT DISCONNECTED";
            emit signalDisconnect();
        }

        if (buf[0] == REFRESH_CLIENT)
        {
            qDebug() << "INSIDE REFRESH CLIENT";

            //Refresh client or song list
            QVector<QString> userList;
            std::string s(bp);
            size_t pos = 0;
            std::string userDelimiter = "&";
            if(s.find(userDelimiter) != std::string::npos)
            {
                pos = s.find(userDelimiter);
                s.erase(pos, userDelimiter.length());
            }
            std::stringstream ss(s);
            std::string token;
            while(std::getline(ss, token, ' '))
            {
                QString tokenString = QString::fromUtf8(token.c_str());
                userList.push_back(tokenString);
            }
            emit updateUserList(userList);
        }

        qDebug() << buf;
    }
}
void ThreadManager::disconnect()
{
    closesocket(sd);
    WSACleanup();
    emit finished();
}
DWORD WINAPI uploadStuff(LPVOID param){
    char * title = (char *)param;
    FILE * upload;
    char buffer[FILEMAX] = { 0 };
    char * buff = buffer;
    int bytesRead;
    if(!(upload = fopen(title, "rb+"))){
            return -1;
    }
    while((bytesRead = fread(buff, sizeof(char), FILEMAX, upload))){
            if(bytesRead != FILEMAX){
                WSAS(sd, buff, bytesRead, 1000);
                break;
            }
            WSAS(accept_socket, buff, 20000, 1000);
    }


}


DWORD WINAPI readStuff(LPVOID param){
    char * title = (char *)param;
    QFile wr(title);
    FILE * fqt;
    DWORD err;
    int len;
    char readBuff[20000] = { 0 };
    HANDLE events[2];
    events[0] = newData;
    events[1] = readDone;
    char * wrt;
    fqt = fopen(title, "wb");
    wrt = readBuff;
    while(1){
        err = WaitForMultipleObjects(2, events, FALSE, INFINITE);
        ResetEvent(newData);
        while(len = c.pop(wrt)){
            fwrite(wrt, sizeof(char), len, fqt);
            printf("head %d tail %d, length: %d\n", c.head, c.tail, len);
            memset(readBuff, '\0', 20000);
        }
        if(err == WAIT_OBJECT_0 + 1){
            wr.close();
            SetEvent(fileDone);
            fclose(fqt);
            break;
        }

    }
}

void ThreadManager::SendDownloadRequest()
{
    HANDLE writeThread;
    DWORD id;
    std::string temp;
    char * dontcare = new char[1024];
    c.init();
    temp = REQ_DOWNLOAD;
    temp += "ec1.m4a";
    char * title = "ec1.m4a";
    sendDataTCP(sd, temp.c_str());
    receiveTCP(sd, dontcare);
    char buf[20000] = { 0 };
    int len;
    writeThread = CreateThread(NULL, 0, readStuff, (void *)title, 0 , &id);
    while((len = WSARead(sd, buf, 2000, 20000))){
        c.push(buf, len);
        SetEvent(newData);
    }
    SetEvent(readDone);
    WaitForSingleObject(fileDone, 20000);
    printf("Done reading");
    fflush(stdout);
}
void ThreadManager::SendUploadRequest()
{
    HANDLE uploadThread;
    std::string temp;
    DWORD id;
    temp = REQ_UPLOAD;
    temp += "download.jpg";
    sendDataTCP(sd, temp.c_str());
    char * tempName = "download.jpg";
    uploadThread = CreateThread(NULL, 0, uploadStuff, (void *)tempName, 0 , &id);


}
void ThreadManager::SendStreamRequest()
{
    std::string temp;
    temp = REQ_STREAM;
    sendDataTCP(sd, temp.c_str());
}
void ThreadManager::SendVoiceRequest()
{
    std::string temp;
    temp = REQ_CHAT;
    sendDataTCP(sd, temp.c_str());
}
void ThreadManager::SendRefreshRequest()
{
    std::string temp;
    temp = REQ_REFRESH;
    sendDataTCP(sd, temp.c_str());
}
