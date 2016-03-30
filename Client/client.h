#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "tcpthreadmanager.h"

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

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_updateSongButton_clicked();
    void on_uploadButton_clicked();
    void on_voiceChatButton_clicked();

    void on_downloadSongButton_clicked();

private:
    Ui::client *ui;
    QThread* receiveTCPThread;
    ThreadManager* receiveTCPWorker;
    QThread* receiveUDPThread;
    ThreadManager* receiveUDPWorker;
    QThread* receiveVoiceChatThread;
    ThreadManager* receiveVoiceChatWorker;
};

#endif // CLIENT_H
