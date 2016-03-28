#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

#include <QObject>
#include <winsock2.h>
#include <windows.h>
#include "clienthandlerthread.h"

class ControlThread : public QObject
{
    Q_OBJECT
public:
    ControlThread(int port);

signals:
    void finished();

public slots:
    void setup();

private:
    WORD wVersionRequested;
    WSADATA WSAData;
    SOCKET listen_socket;
    SOCKET accept_socket;
    SOCKADDR_IN  server;
    SOCKADDR_IN client;
    int m_port;
    int client_len;
    QThread* clientHandlerThread;
    ClientHandlerThread* clientWorker;
    int numberOfClients = 0;
};

#endif // CONTROLTHREAD_H
