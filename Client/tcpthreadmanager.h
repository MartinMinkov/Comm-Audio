#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QDebug>
#include <QWidget>
#include "networkutility.h"

class ThreadManager : public QObject
{
    Q_OBJECT

public:
    explicit ThreadManager(QObject *parent = 0);
    ~ThreadManager();
     void parseUserList(char* bp);

signals:
    void signalConnect(QString ipaddr, QString portnum, QString username);
    void signalDisconnect();
    void signalHandleRequest();
    void updateUserList(QVector<QString> userList);
    void updateSongList(QVector<QString> userList);
    void signalDownload();
    void signalUpload();
    void signalStream();
    void signalVoice();
    void signalRefresh();
    void finished();
    void signalVoiceChat();

public slots:
    void connect(QString ipaddr, QString portnum, QString username);
    void disconnect();
    void handleRequest();
    void SendDownloadRequest();
    void SendUploadRequest();
    void SendStreamRequest();
    void SendVoiceRequest();
    void SendRefreshRequest();
    void setupVoiceChat();

private:
    DWORD ret;
    WSADATA stWSAData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    SOCKET TCPSocket;
    SOCKET AcceptSocket;
    SOCKET VCSocket;
    struct sockaddr_in server;
    struct sockaddr_in client;
    struct hostent *hp;
    LPSOCKET_INFORMATION SI;
};

#endif // THREADMANAGER_H
