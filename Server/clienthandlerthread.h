#ifndef CLIENTHANDLERTHREAD_H
#define CLIENTHANDLERTHREAD_H

#include <QObject>
#include <QVector>

#include "globals.h"
#include "networkutility.h"

#include "filehelper.h"
#include "streamhelper.h"
#include "chathelper.h"

#include "mybuffer.h"

class ClientHandlerThread : public QObject
{
    Q_OBJECT
public:
    ClientHandlerThread(int socket, myBuffer * player);

signals:
    void signalDisconnect();
    void signalUpdateUserList(QVector<QString> userList);
    void finished();

public slots:
    void receiveRequests();
    void disconnect();

private:
    int m_socket;
    filehelper* fHelper;
    streamhelper* sHelper;
    chathelper* cHelper;

    char * clientUsername;
    QString constructUserListString();
    std::string constructSongListString();
    void removeUserFromList();
    void setupStream();
    void startStream();
};
#endif // CLIENTHANDLERTHREAD_H
