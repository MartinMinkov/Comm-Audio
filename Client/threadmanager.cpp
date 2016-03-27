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
        //ExitThread(1);
    }

    //TCP Socket
    if ((sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        qDebug() << "Cannot create TCP socket";
        return;
        //ExitThread(1);
    }



    // Initialize and set up the address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(portnum.toStdString().c_str()));
    if ((hp = gethostbyname(ipaddr.toStdString().c_str())) == NULL)
    {
        formatMessage("Unknown server address");
        return;
        //ExitThread(1);
    }
    // Copy the server address
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    // Connecting to the server
    if (::connect(sd, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        formatMessage("Can't connect to server");
        return;
        //ExitThread(1);
    }

    //Get and send name
    /*if(username != "") {
        sendDataTCP(SI, username.toStdString().c_str());
        qDebug() << username;
    }
    else {
        qDebug() << "No name";
    }*/

    //Get song list

    //Get user list


}

void ThreadManager::formatMessage(const char* message)
{
    qDebug() << message;
}
