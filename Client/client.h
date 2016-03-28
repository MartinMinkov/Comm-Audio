#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QThread>
#include "threadmanager.h"

namespace Ui {
    class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();
    void client::toggleInput(bool state);

private slots:
    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

private:
    Ui::client *ui;
    QThread* receiveThread;
    ThreadManager* receiveWorker;
};

#endif // CLIENT_H
