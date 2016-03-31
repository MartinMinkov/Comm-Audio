#include "tcpthreadmanager.h"

ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{
    TCPSocket = 0;
    VCSocket = 0;
}

ThreadManager::~ThreadManager()
{
    closesocket(TCPSocket);
    TCPSocket = 0;
    VCSocket = 0;
    WSACleanup();
}

void ThreadManager::connect(QString ipaddr, QString portnum, QString username)
{
    qDebug() << "Connect Thread Created";

    if(TCPSocket != 0) {
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
    if ((TCPSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
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

    // Connecting to the server
    if (::connect(TCPSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        formatMessage("Can't connect to server");
        return;
    }

    //Get and send name
    if(username != "") {
        sendDataTCP(TCPSocket, username.toStdString().c_str());
        qDebug() << username;
    }
    else {
        qDebug() << "No name";
    }

    emit signalHandleRequest();
    emit signalVoiceChat();
    emit finished();
}
void ThreadManager::setupVoiceChat()
{
    qDebug() << "Setup Voice Chat Thread Created";

    if(VCSocket != 0 && AcceptSocket != 0) {
        qDebug() << "Socket not null";
        return;
    }
    if ((AcceptSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
           qDebug() << "WSASocket: Failed to get a socket";
           return;
    }
    //Init address structs
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(DEFAULT_VOICE_PORT);

    if (bind(AcceptSocket, (PSOCKADDR)&server, sizeof(server)) == SOCKET_ERROR)
    {
        qDebug() << "bind() failed on server";
        return;
    }
    if (listen(AcceptSocket, 1))
    {
       qDebug() << "listen() failed on tcp server";
       return;
    }
    int client_len = sizeof(client);
    if((VCSocket = accept(AcceptSocket, (struct sockaddr *)&client, &client_len)) == INVALID_SOCKET)
    {
        qDebug() <<  "Can't accept client";
        return;
    }

    // emit voice chat GUI signal
    // emit voice chat recv thread
}
void ThreadManager::handleRequest()
{
    int BytesRead;
    char *bp, buf[PACKET_LEN];
    while (true)
    {
        int bytesToRead = PACKET_LEN;
        char *bp = buf;
        if ((BytesRead = recv(TCPSocket, bp, bytesToRead, 0)) > 0)
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

        if (buf[0] == REFRESH_USER || buf[0] == REFRESH_SONG)
        {
            qDebug() << "INSIDE REFRESH CLIENT";
            parseUserList(bp);
        }
        if (buf[0] ==  REQ_DOWNLOAD)
        {
            //Erase control character
            //Write buffer to local file
        }
        qDebug() << buf;
    }
}
void ThreadManager::disconnect()
{
    closesocket(TCPSocket);
    closesocket(VCSocket);
    WSACleanup();
    emit finished();
}
void ThreadManager::SendDownloadRequest(QString songName)
{
    qDebug() << "Send Download Request is called";
    sendDataTCP(TCPSocket, songName.toStdString().c_str());
}
void ThreadManager::SendUploadRequest()
{
    std::string temp;
    temp = REQ_UPLOAD;
    sendDataTCP(TCPSocket, temp.c_str());
}
void ThreadManager::SendStreamRequest()
{
    std::string temp;
    temp = REQ_STREAM;
    sendDataTCP(TCPSocket, temp.c_str());
}
void ThreadManager::SendVoiceRequest()
{
    std::string temp;
    temp = REQ_CHAT;
    sendDataTCP(TCPSocket, temp.c_str());
}
void ThreadManager::SendRefreshRequest()
{
    qDebug() << "Send Refresh Request is called";
    std::string temp;
    temp = REFRESH_SONG;
    sendDataTCP(TCPSocket, temp.c_str());
}
void ThreadManager::parseUserList(char* bp)
{
    //Refresh client
    bool checkFlag = 0;
    QVector<QString> userList;
    std::string s(bp);
    size_t pos = 0;
    std::string userDelimiter = "&";
    std::string songDelimiter = "*";
    if((s.find(userDelimiter) != std::string::npos))
    {
        checkFlag = 0;
        pos = s.find(userDelimiter);
        s.erase(pos, userDelimiter.length());
    } else if ((s.find(songDelimiter) != std::string::npos))
    {
        checkFlag = 1;
        pos = s.find(songDelimiter);
        s.erase(pos, songDelimiter.length());
    }
    std::stringstream ss(s);
    std::string token;
    while(std::getline(ss, token, ';'))
    {
        QString tokenString = QString::fromUtf8(token.c_str());
        userList.push_back(tokenString);
    }
    if (checkFlag == 0)
        emit updateUserList(userList);
    else if (checkFlag == 1)
        emit updateSongList(userList);
}
