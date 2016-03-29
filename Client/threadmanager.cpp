#include "threadmanager.h"

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

    // Connecting to the server
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

    emit signalHandleRequest();
    emit finished();
}
void ThreadManager::handleRequest()
{
    int BytesRead;
    char *bp, buf[PACKET_LEN];
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

        if (buf[0] == REFRESH_USER || buf[0] == REFRESH_SONG)
        {
            qDebug() << "INSIDE REFRESH CLIENT";
            parseUserList(bp);
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
void ThreadManager::SendDownloadRequest()
{
    std::string temp;
    temp = REQ_DOWNLOAD;
    sendDataTCP(sd, temp.c_str());
}
void ThreadManager::SendUploadRequest()
{
    std::string temp;
    temp = REQ_UPLOAD;
    sendDataTCP(sd, temp.c_str());
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
    temp = REFRESH_SONG;
    sendDataTCP(sd, temp.c_str());
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

