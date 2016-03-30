#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    for(int i= 1; i < 5; i++)
    {
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
    receiveTCPThread = new QThread;
    receiveVoiceChatThread = new QThread;

    receiveTCPWorker = new ThreadManager();
    receiveVoiceChatWorker = new ThreadManager();

    receiveTCPWorker->moveToThread(receiveTCPThread);
    receiveVoiceChatWorker->moveToThread(receiveVoiceChatThread);

    connect(receiveTCPWorker, SIGNAL(signalConnect(QString, QString, QString)), receiveTCPWorker, SLOT(connect(QString, QString, QString)));
    connect(receiveTCPWorker, SIGNAL(updateUserList(QVector<QString>)), this, SLOT(updateUsers(QVector<QString>)));
    connect(receiveTCPWorker, SIGNAL(updateSongList(QVector<QString>)), this, SLOT(updateSongs(QVector<QString>)));
    connect(receiveTCPWorker, SIGNAL(signalHandleRequest()), receiveTCPWorker, SLOT(handleRequest()));
    connect(receiveTCPWorker, SIGNAL(finished()), receiveTCPThread, SLOT(quit()));

    connect(receiveVoiceChatWorker, SIGNAL(signalVoiceChat()), receiveVoiceChatWorker, SLOT(setupVoiceChat()));
    connect(receiveVoiceChatWorker, SIGNAL(finished()), receiveTCPThread, SLOT(quit()));

    receiveTCPThread->start();
    receiveVoiceChatThread->start();

    client::toggleInput(false);
    ui->connectStatus->setText("Connected");
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, true);
    }

    emit receiveTCPWorker->signalConnect(ipaddr, portnum, username);
}

void client::on_disconnectButton_clicked()
{
    connect(receiveTCPWorker, SIGNAL(signalDisconnect()), receiveTCPWorker, SLOT(disconnect()));
    connect(receiveVoiceChatWorker, SIGNAL(signalDisconnect()), receiveVoiceChatWorker, SLOT(disconnect()));

    //disconnect
    emit receiveTCPWorker->signalDisconnect();
    emit receiveVoiceChatWorker->signalDisconnect();

    client::toggleInput(true);
    ui->connectStatus->setText("Disconnected");
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, false);
    }
}
void client::on_updateSongButton_clicked()
{
    connect(receiveTCPWorker, SIGNAL(signalRefresh()), receiveTCPWorker, SLOT(SendRefreshRequest()));
    emit receiveTCPWorker->signalRefresh();
}

void client::on_uploadButton_clicked()
{
    //emit receiveWorker->signalUpload();
}

void client::on_voiceChatButton_clicked()
{
    //emit receiveWorker->signalUpload();
}
void client::on_downloadSongButton_clicked()
{

}

void client::toggleInput(bool state)
{
    ui->connectButton->setEnabled(state);
    ui->disconnectButton->setEnabled(!state);
    ui->portfield->setDisabled(!state);
    ui->ipfield->setDisabled(!state);
    ui->nameField->setDisabled(!state);
}
void client::updateUsers(QVector<QString> userList)
{
    ui->connectedWidget->clear();
    for(auto& user : userList){
       ui->connectedWidget->addItem(user);
    }
}
void client::updateSongs(QVector<QString> userList)
{
    ui->downloadFileWidget->clear();
    for(auto& user : userList){
       ui->connectedWidget->addItem(user);
    }
}
