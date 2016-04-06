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
    void signalUDPWorker();
    void signalDisconnect();

public slots:
    void UDPWorker();
    void disconnect();
private:
    struct ip_mreq stMreq;         /* Multicast interface structure */
};

#endif // UDPTHREADMANAGER_H
