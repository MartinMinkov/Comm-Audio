#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QDebug>
#include <winsock2.h>
#include <windows.h>

class ThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit ThreadManager(QObject *parent = 0);
    ~ThreadManager();
    void formatMessage(const char* message);

signals:
    void connect(QString ipaddr, QString portnum, QString username);

public slots:


private:
    DWORD ret;
    WSADATA stWSAData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    SOCKET sd;
    struct sockaddr_in server;
    struct hostent *hp;
    //LPSOCKET_INFORMATION SI;
};

#endif // THREADMANAGER_H
