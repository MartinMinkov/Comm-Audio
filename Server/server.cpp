#include "server.h"
#include "networkutility.h"
#include "ui_server.h"

server::server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    toggleConnected(false);
}

server::~server()
{
    delete ui;
}

void server::initControlThread(){

    int port = getPortNumber();

    qDebug() << QString::number(port);

    controlThread = new QThread;
    controlWorker = new ControlThread();
    controlWorker->moveToThread(controlThread);
    connect(controlWorker, SIGNAL(signalSetup(int)), controlWorker, SLOT(setup(int)));
    connect(controlWorker, SIGNAL(signalDisconnect()), controlWorker, SLOT(disconnect()));
    connect(controlWorker, SIGNAL(finished()), controlThread, SLOT(quit()));
    connect(controlWorker, SIGNAL(finished()), controlWorker, SLOT(deleteLater()));
    connect(controlThread, SIGNAL(finished()), controlThread, SLOT(deleteLater()));
    controlThread->start();

    toggleConnected(true);

    emit controlWorker->signalSetup(port);
}

void server::on_bStartServer_clicked(){
    initControlThread();
}

void server::on_bStopServer_clicked(){
    emit controlWorker->disconnect();
    toggleConnected(false);
}


int server::getPortNumber(){
    int port = ui->etPort->text().toInt();
    if(port == 0){
        port = DEFAULT_PORT;
    }
    return port;
}

void server::toggleConnected(bool state){
    ui->bStartServer->setEnabled(!state);
    ui->bStopServer->setEnabled(state);
}
