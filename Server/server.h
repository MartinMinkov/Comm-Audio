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

private:
    Ui::server *ui;
    QThread* controlThread;
    ControlThread* controlWorker;
    void initControlThread();

private slots:
    void on_bStartServer_clicked();
};

#endif // SERVER_H
