#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "tcpthreadmanager.h"
#include "udpthreadmanager.h"
#include <QAudio>

namespace Ui {
    class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();
    void toggleInput(bool state);

public slots:
    void updateUsers(QVector<QString> userList);
    void updateSongs(QVector<QString> userList);
    void updateCallLabel(QString);
    void handleStateChanged(QAudio::State newState);

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_updateSongButton_clicked();
    void on_uploadButton_clicked();
    void on_voiceChatButton_clicked();
    void on_downloadSongButton_clicked();
    void on_updateVoiceUsersButton_clicked();
    void on_playStreamButton_clicked();
    void on_rewindStreamButton_clicked();
    void on_stopStreamButton_clicked();

private:
    Ui::client *ui;
    QThread* receiveTCPThread;
    ThreadManager* receiveTCPWorker;

    QThread* receiveUDPThread;
    ThreadManager* receiveUDPWorker;

    QThread* receiveVoiceChatThread;
    ThreadManager* receiveVoiceChatWorker;

    QThread* streamUDPThread;
    UDPThreadManager* streamUDPWorker;
};

#endif // CLIENT_H
