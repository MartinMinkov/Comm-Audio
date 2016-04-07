#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QThread>
#include <QDebug>
#include <QFileDialog>
#include <QStringListModel>
#include "playermanager.h"
#include "controlthread.h"
#include "clienthandlerthread.h"
#include "streamhandlerthread.h"

#include "networkutility.h"
#include "ui_server.h"
#include "globals.h"
#include "mybuffer.h"
#include <windows.h>

namespace Ui {
    class server;
}

class server : public QWidget
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = 0);
    ~server();
    int getPortNumber();

public slots:
    void updateUserList(QVector<QString> userList);
    void createClientThread(int socket);

signals:
    void signalSetup(int port);

private:
    Ui::server *ui;
    void initControlThread();
    void toggleConnected(bool state);
    void setupPlaylistTable();
    QStringList playlist;
    playerManager * play;
    QThread* controlThread;
    ControlThread* controlWorker;
    QThread* clientHandlerThread;
    ClientHandlerThread* clientWorker;
    QThread* streamThread;
    streamhandlerthread* streamHandlerWorker;


private slots:
    void on_bStartServer_clicked();
    void on_bStopServer_clicked();
    void on_bAddSongs_clicked();
    void on_button_start_stream_clicked();
};



#endif // SERVER_H
