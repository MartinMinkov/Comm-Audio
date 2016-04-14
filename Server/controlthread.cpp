#include "controlthread.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: controlthread.cpp
--
-- FUNCTIONS:
-- ControlThread::ControlThread(QObject *parent) : QObject(parent)
-- ControlThread::~ControlThread()
-- void ControlThread::setup(int port)
-- void ControlThread::disconnect()
--
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
--
----------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ControlThread
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: ControlThread(QObject *parent) : QObject(parent)
--
-- NOTES: Constructor for client control thread.
----------------------------------------------------------------------------------------------------------------------*/
ControlThread::ControlThread(QObject *parent) : QObject(parent){
    listen_socket = 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~ControlThread
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: ControlThread::~ControlThread()
--
-- NOTES: Constructor for client control thread.
----------------------------------------------------------------------------------------------------------------------*/
ControlThread::~ControlThread(){
    closesocket(listen_socket);
    listen_socket = 0;
    WSACleanup();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setup
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: setup(int port)
--
-- NOTES: Sets up the socket setup, binding, and accept calls, and creates a new thread for the client when he
-- connects.
----------------------------------------------------------------------------------------------------------------------*/
void ControlThread::setup(int port){
    networkutility::debugMessage("inside setup()");

    //Startup WSA
    wVersionRequested = MAKEWORD(2, 2);
    if (WSAStartup(wVersionRequested, &WSAData) != 0) {
        networkutility::debugMessage("WSAStartup: DLL not found");
        return;
    }

    //Create socket
    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        networkutility::debugMessage("WSASocket: Failed to get a socket");
        return;
    }

    //Init address structs
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if (port == 0) {
        server.sin_port = htons(DEFAULT_PORT);
    }
    else {
        server.sin_port = htons(port);
    }

    if (bind(listen_socket, (PSOCKADDR)&server,
        sizeof(server)) == SOCKET_ERROR)
    {
        networkutility::debugMessage("bind() failed on server");
        return;
    }

    if (listen(listen_socket, MAXCONNECTIONS))
    {
        networkutility::debugMessage("listen() failed on tcp server");
        return;
    }

    networkutility::debugMessage("passed listen");

    while(1){
        client_len = sizeof(client);

        if((accept_socket = accept(listen_socket, (struct sockaddr *)&client, &client_len)) == SOCKET_ERROR){
            networkutility::debugMessage("Can't accept TCP client");
            return;
        }

        //get the client IP so we can use it for voice chat
        QString clientIP = inet_ntoa(client.sin_addr);

        networkutility::debugMessage("passed accept");

        networkutility::debugMessage("create thread here for the new client");

        //send a signal here to the main thread to create the handler thread, instead of creating it here
        emit signalCreateClientThread(accept_socket, clientIP);

        //keep track of clients?
        numberOfClients++;
        qDebug() << numberOfClients;
    }

    emit finished();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: disconnect
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: setup(int port)
--
-- NOTES: Handles cleanup and disconnecting of the server.
----------------------------------------------------------------------------------------------------------------------*/
void ControlThread::disconnect(){
    networkutility::debugMessage("disconnecting control thread...");
    closesocket(listen_socket);
    WSACleanup();
    emit finished();
}
