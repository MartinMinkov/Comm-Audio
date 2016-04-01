#include "server.h"
#include "networkutility.h"
#include "ui_server.h"
#include "globals.h"

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
    connect(controlWorker, SIGNAL(signalCreateClientThread(int)), this, SLOT(createClientThread(int)));

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

void server::on_bAddSongs_clicked(){
    QFileDialog addSongDialog(this);
    addSongDialog.setFileMode(QFileDialog::ExistingFiles);

    //get the currently selected songs
    QStringList newSongsList;
    newSongsList = addSongDialog.getOpenFileNames(this, tr("Add Songs"), QDir::currentPath(), tr("Song Types (*.wav *.mp3)"));

    for(QStringList::iterator it = newSongsList.begin(); it != newSongsList.end(); ++it){
        QFileInfo fi(*it);
        *it = fi.fileName();
        //add new songs to existing playlist
        playlist.push_back(*it);
    }

    playlistModel = new QStringListModel(this);

    playlistModel->setStringList(playlist);
    ui->playlistView->setModel(playlistModel);
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

void server::createClientThread(int socket){

    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    clientHandlerThread = new QThread;
    clientWorker = new ClientHandlerThread(socket);
    clientWorker->moveToThread(clientHandlerThread);
    connect(clientHandlerThread, SIGNAL(started()), clientWorker, SLOT(receiveRequests()));
    //TODO: connect start signal with a slot to create Colin's thread (update song list)
    connect(clientWorker, SIGNAL(signalUpdateUserList(QVector<QString>)), this, SLOT(updateUserList(QVector<QString>)));
    connect(clientWorker, SIGNAL(signalDisconnect()), clientWorker, SLOT(disconnect()));
    connect(clientWorker, SIGNAL(finished()), clientHandlerThread, SLOT(quit()));
    connect(clientWorker, SIGNAL(finished()), clientWorker, SLOT(deleteLater()));
    connect(clientHandlerThread, SIGNAL(finished()), clientHandlerThread, SLOT(deleteLater()));

    clientHandlerThread->start();
}

void server::updateUserList(QVector<QString> userList){
    ui->userView->clear();
    for(auto& user : userList){
        ui->userView->addItem(user);
    }
}
