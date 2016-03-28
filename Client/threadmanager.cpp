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
    temp = REQ_REFRESH;
    sendDataTCP(sd, temp.c_str());
}
