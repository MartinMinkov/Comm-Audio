#include "controlthread.h"
#include "server.h"

ControlThread::ControlThread(int port)
{
    m_port = port;
}

void ControlThread::setup(){
    qDebug("inside setup()");

    //Startup WSA
    wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &WSAData) != 0) {
        qDebug("WSAStartup: DLL not found");
        return;
    }

    //Create socket

    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        qDebug("WSASocket: Failed to get a socket");
        return;
    }

    //Init address structs
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (m_port == 0) {
        server.sin_port = htons(DEFAULT_PORT);
    }
    else {
        server.sin_port = htons(m_port);
    }

    if (bind(listen_socket, (PSOCKADDR)&server,
        sizeof(server)) == SOCKET_ERROR)
    {
        qDebug("bind() failed on server");
        return;
    }

    if (listen(listen_socket, MAXCONNECTIONS))
    {
        qDebug("listen() failed on tcp server");
        return;
    }

    qDebug("passed listen");

    while(1){
        client_len = sizeof(client);

        if((accept_socket = accept(listen_socket, (struct sockaddr *)&client, &client_len)) == -1){
            qDebug("Can't accept client");
            return;
        }

        qDebug("create thread here for the new client");
        clientHandlerThread = new QThread;
        clientWorker = new ClientHandlerThread(accept_socket);
        clientWorker->moveToThread(clientHandlerThread);
        connect(clientHandlerThread, SIGNAL(started()), clientWorker, SLOT(receiveRequests()));
        connect(clientWorker, SIGNAL(finished()), clientHandlerThread, SLOT(quit()));
        connect(clientWorker, SIGNAL(finished()), clientWorker, SLOT(deleteLater()));
        connect(clientHandlerThread, SIGNAL(finished()), clientHandlerThread, SLOT(deleteLater()));

        clientHandlerThread->start();

        //keep track of clients?
        numberOfClients++;
        qDebug() << numberOfClients;
    }

    emit finished();
}
