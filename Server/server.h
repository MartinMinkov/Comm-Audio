#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QThread>
#include <QDebug>
#include "controlthread.h"

#define DEFAULT_PORT 7000
#define DATA_BUFSIZE 65000
#define PACKET_LEN 1024

#define REQ_DOWNLOAD v
#define REQ_UPLOAD ^
#define REQ_STREAM !
#define REQ_CHAT ?
#define REQ_REFRESH %

#define MAXCONNECTIONS 10

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
    static void debugMessage(const char* message);

private:
    Ui::server *ui;
    QThread* controlThread;
    ControlThread* controlWorker;
    void initControlThread();

    void sendDataTCP(SOCKET sd, const char* message);
    bool receiveTCP(SOCKET sd, char* message);

private slots:
    void on_bStartServer_clicked();
};

#endif // SERVER_H
