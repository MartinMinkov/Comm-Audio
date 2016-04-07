#include "server.h"
HANDLE loveQt;
myBuffer player(accept_socket);
server::server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    setupPlaylistTable();
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

    streamThread = new QThread;
    streamHandlerWorker = new streamhandlerthread();
    streamHandlerWorker->moveToThread(streamThread);

    //emit
    connect(streamHandlerWorker, SIGNAL(signalInitMutliCast()), streamHandlerWorker, SLOT(initMultiCast()));

    //start
    streamThread->start();
    emit streamHandlerWorker->signalInitMutliCast();

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

    //repopulate the model with new songs
    int number = 0;
    for(auto it = playlist.begin(); it != playlist.end(); ++it, number++){
        playlistModel->setItem(number, 0, new QStandardItem(QString::number(number+1)));
        playlistModel->setItem(number, 1, new QStandardItem(*it));
        //add duration of the song to column 2
    }

    ui->playlistView->setModel(playlistModel);
    //not sure why it only works if i resize the width here
    ui->playlistView->setColumnWidth(0, 70);
    ui->playlistView->setColumnWidth(1, 350);
    ui->playlistView->setColumnWidth(2, 90);
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
    clientWorker = new ClientHandlerThread(socket, &player);
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
        user.remove('\n');
        ui->userView->addItem(user);
    }
    ui->label_server_client_number->setText(QString::number(userList.size()));
}

void server::setupPlaylistTable(){
    playlistModel = new QStandardItemModel();
    playlistModel->setColumnCount(3);

    QStandardItem* header0 = new QStandardItem("Number");
    QStandardItem* header1 = new QStandardItem("Song Name");
    QStandardItem* header2 = new QStandardItem("Duration");

    playlistModel->setHorizontalHeaderItem(0,header0);
    playlistModel->setHorizontalHeaderItem(1,header1);
    playlistModel->setHorizontalHeaderItem(2,header2);

    playlistModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    playlistModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    playlistModel->setHeaderData(2, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    ui->playlistView->verticalHeader()->hide();

}
DWORD WINAPI doServer(LPVOID param){
    //myBuffer * player;
    //player = new myBuffer(accept_socket);
    //player->startPlayer();
    return NULL;
}

void server::on_button_start_stream_clicked()
{
    DWORD id;
    play = new playerManager();
    play->startSong("ec1.wav");

    //loveQt = CreateThread(NULL, 0, doServer, (void *)0, 0, &id);
   // player.startPlayer();
}
