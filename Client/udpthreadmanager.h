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
    void initalizeVoiceChatSockets();

signals:
    void signalUDPWorker(SOCKET sd, struct sockaddr_in);
    void signalDisconnect();

public slots:
    void UDPWorker(SOCKET sd, struct sockaddr_in);
    void disconnect();
private:
    struct ip_mreq stMreq;         /* Multicast interface structure */
    struct sockaddr_in voiceChatReceive;
    struct hostent *hp;
};
extern struct sockaddr_in voiceChatSend;
#endif // UDPTHREADMANAGER_H
