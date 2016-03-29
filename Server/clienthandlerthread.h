#ifndef CLIENTHANDLERTHREAD_H
#define CLIENTHANDLERTHREAD_H

#include <QObject>
#include "filehelper.h"
#include "streamhelper.h"
#include "chathelper.h"

class ClientHandlerThread : public QObject
{
    Q_OBJECT
public:
    ClientHandlerThread(int socket);

signals:
    void signalDisconnect();
    void finished();

public slots:
    void receiveRequests();
    void disconnect();

private:
    int m_socket;
    filehelper* fHelper;
    streamhelper* sHelper;
    chathelper* cHelper;
};

#endif // CLIENTHANDLERTHREAD_H
