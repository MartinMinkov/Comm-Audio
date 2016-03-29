#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QThread>
#include <QDebug>
#include "controlthread.h"


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


signals:
    void signalSetup(int port);

private:
    Ui::server *ui;
    QThread* controlThread;
    ControlThread* controlWorker;
    void initControlThread();
    void toggleConnected(bool state);


private slots:
    void on_bStartServer_clicked();
    void on_bStopServer_clicked();
};

#endif // SERVER_H
