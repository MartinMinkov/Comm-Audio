#include "server.h"
#include "networkutility.h"
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
