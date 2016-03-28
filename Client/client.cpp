#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, false);
    }
}

client::~client()
{
    delete ui;
}

void client::on_connectButton_clicked()
{
    QString ipaddr = ui->ipfield->text();
    QString portnum = ui->portfield->text();
    QString username = ui->nameField->text();

    //connect
    receiveThread = new QThread;
    receiveWorker = new ThreadManager();
    receiveWorker->moveToThread(receiveThread);
    connect(receiveWorker, SIGNAL(signalConnect(QString, QString, QString)), receiveWorker, SLOT(connect(QString, QString, QString)));
    connect(receiveWorker, SIGNAL(signalDisconnect()), receiveWorker, SLOT(disconnect()));
    connect(receiveWorker, SIGNAL(signalHandleRequest()), receiveWorker, SLOT(handleRequest()));
    connect(receiveWorker, SIGNAL(finished()), receiveThread, SLOT(quit()));
    receiveThread->start();

    client::toggleInput(false);

    ui->connectStatus->setText("Connected");
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, true);
    }

    emit receiveWorker->signalConnect(ipaddr, portnum, username);
    emit receiveWorker->signalHandleRequest();
}

void client::on_disconnectButton_clicked()
{
    //disconnect
    client::toggleInput(true);
    emit receiveWorker->signalDisconnect();

    ui->connectStatus->setText("Disconnected");
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, false);
    }
}
void client::toggleInput(bool state)
{
    ui->connectButton->setEnabled(state);
    ui->disconnectButton->setEnabled(!state);
    ui->portfield->setDisabled(!state);
    ui->ipfield->setDisabled(!state);
    ui->nameField->setDisabled(!state);
}
