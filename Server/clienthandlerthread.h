#ifndef CLIENTHANDLERTHREAD_H
#define CLIENTHANDLERTHREAD_H

#include <QObject>

class ClientHandlerThread : public QObject
{
    Q_OBJECT
public:
    ClientHandlerThread(int socket);

signals:
    void finished();

public slots:
    void receiveRequests();

private:
    int m_socket;
};

#endif // CLIENTHANDLERTHREAD_H
