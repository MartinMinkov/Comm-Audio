#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QDebug>
#include <QWidget>
#include <QThread>
#include "circlebuff.h"
#include "networkutility.h"
#include "globals.h"
#include "udpthreadmanager.h"
#include <QMessageBox>

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
    void signalUpload(QString songName);
    void signalVoice();
    void signalSongRefresh();
    void signalVoiceRefresh();
    void finished();
    void signalVoiceChat();
    void signalGetVoiceChatIP(QString username);
    void updateCaller(QString);
    void updateStatusBar(bool connected);
    void signalVoiceConnect(QString clientIP);
    void signalStartPlayer();
    void signalCallNotification();
    void songVote(char * song);
    void signalDownloadStatus(int state);
    void signalUploadStatus(int state);
    void signalUpdateVoiceChatUser(QString);

public slots:
    void connect(QString ipaddr, QString portnum, QString username);
    void disconnect();
    bool handleRequest();
    void SendDownloadRequest(QString songName);
    void SendUploadRequest(QString songName);
    //DWORD WINAPI readStuff(LPVOID param);
    void SendVoiceRequest();
    void requestVoiceChatIP(QString username);
    void SendSongRefreshRequest();
    void SendVoiceRefreshRequest();
    void setupVoiceChat();
    void VoiceConnect(QString clientIP);
    void voteForSong(char * song);

private:
    UDPThreadManager udp;
    DWORD ret;
    WSADATA stWSAData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    struct sockaddr_in server;
    struct sockaddr_in VCserver;
    struct sockaddr_in voiceChatClient;
    struct hostent *hp;
    QString mUsername;
};

DWORD WINAPI readStuff(LPVOID param);
#endif // THREADMANAGER_H
