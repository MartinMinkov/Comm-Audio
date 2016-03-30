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
    receiveThread = new QThread;
    receiveWorker = new ThreadManager();
    receiveWorker->moveToThread(receiveThread);
    connect(receiveWorker, SIGNAL(signalConnect(QString, QString, QString)), receiveWorker, SLOT(connect(QString, QString, QString)));
    connect(receiveWorker, SIGNAL(updateUserList(QVector<QString>)), this, SLOT(updateUsers(QVector<QString>)));
    connect(receiveWorker, SIGNAL(signalDisconnect()), receiveWorker, SLOT(disconnect()));
    connect(receiveWorker, SIGNAL(signalRefresh()), receiveWorker, SLOT(SendRefreshRequest()));
    connect(receiveWorker, SIGNAL(signalHandleRequest()), receiveWorker, SLOT(handleRequest()));
    connect(receiveWorker, SIGNAL(updateUserList(QVector<QString>)), this, SLOT(updateUsers(QVector<QString>)));
    connect(receiveWorker, SIGNAL(updateSongList(QVector<QString>)), this, SLOT(updateSongs(QVector<QString>)));
    connect(receiveWorker, SIGNAL(finished()), receiveThread, SLOT(quit()));
    receiveThread->start();

    client::toggleInput(false);

    ui->connectStatus->setText("Connected");
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, true);
    }

    emit receiveWorker->signalConnect(ipaddr, portnum, username);
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
void client::on_updateSongButton_clicked()
{

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
    qDebug() << "Update song button";
    connect(receiveWorker, SIGNAL(signalDownload(QString)), receiveWorker, SLOT(SendDownloadRequest(QString)));
    QString songName = QString("%1 %2").arg(REQ_DOWNLOAD).arg(ui->downloadFileWidget->currentItem()->text());
    emit receiveWorker->signalDownload(songName);
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
