#include "server.h"
#include "ui_server.h"

server::server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    //start tcp control thread
}

server::~server()
{
    delete ui;
}

void server::initControlThread(){

    int port = getPortNumber();

    qDebug() << QString::number(port);

    controlThread = new QThread;
    controlWorker = new ControlThread(port);
    controlWorker->moveToThread(controlThread);
    connect(controlThread, SIGNAL(started()), controlWorker, SLOT(setup()));
    connect(controlWorker, SIGNAL(finished()), controlThread, SLOT(quit()));
    connect(controlWorker, SIGNAL(finished()), controlWorker, SLOT(deleteLater()));
    connect(controlThread, SIGNAL(finished()), controlThread, SLOT(deleteLater()));
    controlThread->start();
}

void server::on_bStartServer_clicked(){
    initControlThread();
}

int server::getPortNumber(){
    int port = ui->etPort->text().toInt();
    if(port == 0){
        port = DEFAULT_PORT;
    }
    return port;
}

void server::debugMessage(const char* message){
    qDebug() << message;
}

void server::sendDataTCP(SOCKET sd, const char* message){
    if(send(sd, message, PACKET_LEN, 0) == SOCKET_ERROR){
        return;
    }
    debugMessage("Sending data to client");
}

bool server::receiveTCP(SOCKET sd, char* message){
    if(recv(sd, message, PACKET_LEN, 0) == SOCKET_ERROR){
        return FALSE;
    }
    return TRUE;
}
