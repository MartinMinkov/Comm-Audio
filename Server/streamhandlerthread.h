#ifndef STREAMHANDLERTHREAD_H
#define STREAMHANDLERTHREAD_H

#include <QObject>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "globals.h"
#include "networkutility.h"

class streamhandlerthread : public QObject
{
    Q_OBJECT
public:
    explicit streamhandlerthread(QObject *parent = 0);
    void sendToMultiCast();

signals:
    void signalInitMutliCast();

public slots:
    void initMultiCast();

private:
    SOCKET StreamSocket;
    SOCKADDR_IN  stLclAddr;
    SOCKADDR_IN  client;
    struct ip_mreq stMreq;        /* Multicast interface structure */
};

#endif // STREAMHANDLERTHREAD_H
