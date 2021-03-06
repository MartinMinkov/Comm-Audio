#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

#include <QObject>
#include <winsock2.h>
#include <windows.h>

#include "clienthandlerthread.h"
#include "globals.h"
#include "networkutility.h"

class ControlThread : public QObject
{
    Q_OBJECT
public:
    explicit ControlThread(QObject *parent = 0);
    ~ControlThread();

signals:
    void signalSetup(int port);
    void signalDisconnect();
    void signalCreateClientThread(int socket, QString clientIP);
    void finished();

public slots:
    void setup(int port);
    void disconnect();

private:
    WORD wVersionRequested;
    WSADATA WSAData;

   // SOCKET accept_socket;
    SOCKET listen_socket;
    SOCKET accept_socket;
    SOCKADDR_IN  server;
    SOCKADDR_IN client;
    int client_len;

    int numberOfClients = 0;
};

#endif // CONTROLTHREAD_H
