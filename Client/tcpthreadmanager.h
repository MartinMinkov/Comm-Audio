#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QDebug>
#include <QWidget>
#include <QThread>
#include "circlebuff.h"
#include "networkutility.h"

#define FILEMAX 20000

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
    void signalDownload(QString songName);
    void signalUpload();
    void signalStream();
    void signalVoice();
    void signalSongRefresh();
    void signalVoiceRefresh();
    void finished();
    void signalVoiceChat();
    void updateCaller(QString);

public slots:
    void connect(QString ipaddr, QString portnum, QString username);
    void disconnect();
    bool handleRequest();
    void SendDownloadRequest(QString songName);
    void SendUploadRequest(QString songName);
    void SendStreamRequest();
    //DWORD WINAPI readStuff(LPVOID param);
    void SendVoiceRequest();
    void SendSongRefreshRequest();
    void SendVoiceRefreshRequest();
    void setupVoiceChat();

private:
    DWORD ret;
    WSADATA stWSAData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    struct sockaddr_in server;
    struct sockaddr_in voiceChatClient;
    struct hostent *hp;
    LPSOCKET_INFORMATION SI;
};

DWORD WINAPI readStuff(LPVOID param);
#endif // THREADMANAGER_H
