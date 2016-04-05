#ifndef UDPTHREADMANAGER_H
#define UDPTHREADMANAGER_H

#include <QObject>
#include <QDebug>
#include <QWidget>
#include <QThread>
#include "networkutility.h"

class UDPThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit UDPThreadManager(QObject *parent = 0);
    void initMultiCastSock();
    void receiveStream();

signals:

public slots:

private:
    struct ip_mreq stMreq;         /* Multicast interface structure */
    struct sockaddr_in server;
};

#endif // UDPTHREADMANAGER_H
