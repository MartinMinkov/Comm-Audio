#include "server.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: server.cpp
--
-- FUNCTIONS:
-- server(QWidget *parent)
-- ~server()
-- initControlThread()
-- on_bStartServer_clicked()
-- on_bStopServer_clicked()
-- on_bAddSongs_clicked()
-- getPortNumber()
-- toggleConnected(bool state)
-- createClientThread(int socket, QString clientIP)
-- updateUserList(QVector<QString> userList)
-- setupPlaylistTable()
-- on_button_start_stream_clicked()
-- updateCurrentlyPlayingLabel(QString songName)
--
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
--
----------------------------------------------------------------------------------------------------------------------*/
myBuffer player(accept_socket);

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: server
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
--
-- NOTES: Constructor for setting up the server window.
----------------------------------------------------------------------------------------------------------------------*/
server::server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);
    isPlaying = false;
    isConnected = false;
    setupPlaylistTable();
    toggleConnected(false);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ~server
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: ~server()
--
-- NOTES: Destructor for the server.
----------------------------------------------------------------------------------------------------------------------*/
server::~server()
{
    delete ui;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: initControlThread
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: initControlThread()
--
-- NOTES: Connects the necessary thread signals with the slots.
----------------------------------------------------------------------------------------------------------------------*/
void server::initControlThread(){

    int port = getPortNumber();

    qDebug() << QString::number(port);

    controlThread = new QThread;
    controlWorker = new ControlThread();
    controlWorker->moveToThread(controlThread);
    connect(controlWorker, SIGNAL(signalSetup(int)), controlWorker, SLOT(setup(int)), Qt::UniqueConnection);
    connect(controlWorker, SIGNAL(signalCreateClientThread(int, QString)), this, SLOT(createClientThread(int, QString)), Qt::UniqueConnection);

    connect(controlWorker, SIGNAL(signalDisconnect()), controlWorker, SLOT(disconnect()), Qt::UniqueConnection);
    connect(controlWorker, SIGNAL(finished()), controlThread, SLOT(quit()), Qt::UniqueConnection);
    connect(controlWorker, SIGNAL(finished()), controlWorker, SLOT(deleteLater()), Qt::UniqueConnection);
    connect(controlThread, SIGNAL(finished()), controlThread, SLOT(deleteLater()), Qt::UniqueConnection);
    controlThread->start();

    toggleConnected(true);

    emit controlWorker->signalSetup(port);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_bStartServer_clicked
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: on_bStartServer_clicked()
--
-- NOTES: handles the logic for clicking the start server button
----------------------------------------------------------------------------------------------------------------------*/
void server::on_bStartServer_clicked(){
    initControlThread();

    streamThread = new QThread;
    streamHandlerWorker = new streamhandlerthread();
    streamHandlerWorker->moveToThread(streamThread);

    //emit
    connect(streamHandlerWorker, SIGNAL(signalInitMutliCast()), streamHandlerWorker, SLOT(initMultiCast()), Qt::UniqueConnection);


    //start
    streamThread->start();
    emit streamHandlerWorker->signalInitMutliCast();

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_bStopServer_clicked
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: on_bStopServer_clicked()
--
-- NOTES: handles the logic for clicking the stop server button
----------------------------------------------------------------------------------------------------------------------*/
void server::on_bStopServer_clicked(){
    emit controlWorker->disconnect();
    toggleConnected(false);


}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_bAddSongs_clicked
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: on_bAddSongs_clicked()
--
-- NOTES: handles the logic for clicking the add songs button
----------------------------------------------------------------------------------------------------------------------*/
void server::on_bAddSongs_clicked(){
    QFileDialog addSongDialog(this);
    addSongDialog.setFileMode(QFileDialog::ExistingFiles);

    //get the currently selected songs
    QStringList newSongsList;
    newSongsList = addSongDialog.getOpenFileNames(this, tr("Add Songs"), QDir::currentPath(), tr("Song Types (*.wav)"));

    for(QStringList::iterator it = newSongsList.begin(); it != newSongsList.end(); ++it){
        QFileInfo fi(*it);
        //add new songs to existing playlist
        playlist.push_back(fi.fileName());
        playlistWithPath.push_back(fi.filePath());
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
    ui->playlistView->setColumnWidth(1, 440);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getPortNumber
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: getPortNumber()
--
-- NOTES: Logic to grab the port number from the input field.
----------------------------------------------------------------------------------------------------------------------*/
int server::getPortNumber(){
    int port = ui->etPort->text().toInt();
    if(port == 0){
        port = DEFAULT_PORT;
    }
    return port;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: toggleConnected
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: toggleConnected()
--
-- NOTES: Toggle the set of buttons depending on the state of the server.
----------------------------------------------------------------------------------------------------------------------*/
void server::toggleConnected(bool state){
    ui->bStartServer->setEnabled(!state);
    ui->bStopServer->setEnabled(state);
    ui->label_server_status->setText(state ? "Status: Running" : "Status: Stopped");
    ui->label_server_status->setStyleSheet(state ?
                                               "#label_server_status { color: #72FFF7; }"
                                             : "#label_server_status { color: #E4144C; }");
    if(state){
        if(ui->etPort->text().toInt() == 0){
            ui->etPort->setText(QString::number(DEFAULT_PORT));
        }
    } else {
        ui->etPort->clear();
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: createClientThread
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: createClientThread()
--
-- NOTES: Creates the client thread and connects the necessary signals and slots.
----------------------------------------------------------------------------------------------------------------------*/
void server::createClientThread(int socket, QString clientIP){

    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    clientHandlerThread = new QThread;
    clientWorker = new ClientHandlerThread(socket, &player, clientIP);
    clientWorker->moveToThread(clientHandlerThread);
    connect(clientHandlerThread, SIGNAL(started()), clientWorker, SLOT(receiveRequests()), Qt::UniqueConnection);
    //TODO: connect start signal with a slot to create Colin's thread (update song list)

    connect(clientWorker, SIGNAL(signalUpdateUserList(QVector<QString>)), this, SLOT(updateUserList(QVector<QString>)), Qt::UniqueConnection);
    connect(clientWorker, SIGNAL(signalHandlerDisconnect()), clientWorker, SLOT(disconnect()), Qt::UniqueConnection);
    connect(clientWorker, SIGNAL(finished()), clientHandlerThread, SLOT(quit()), Qt::UniqueConnection);
    connect(clientWorker, SIGNAL(finished()), clientWorker, SLOT(deleteLater()), Qt::UniqueConnection);
    connect(clientHandlerThread, SIGNAL(finished()), clientHandlerThread, SLOT(deleteLater()), Qt::UniqueConnection);

    clientHandlerThread->start();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateUserList
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: updateUserList(QVector<QString> userList)
--
-- NOTES: Updates the user list and updates the GUI widget.
----------------------------------------------------------------------------------------------------------------------*/
void server::updateUserList(QVector<QString> userList){
    ui->userView->clear();
    for(auto& user : userList){
        user.remove('\n');
        ui->userView->addItem(user);
    }
    ui->label_server_client_number->setText(QString::number(userList.size()));
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setupPlaylistTable
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: setupPlaylistTable()
--
-- NOTES: Sets up the table columns and size for the GUI
----------------------------------------------------------------------------------------------------------------------*/
void server::setupPlaylistTable(){
    playlistModel = new QStandardItemModel();
    playlistModel->setColumnCount(2);

    QStandardItem* header0 = new QStandardItem("Number");
    QStandardItem* header1 = new QStandardItem("Song Name");

    playlistModel->setHorizontalHeaderItem(0,header0);
    playlistModel->setHorizontalHeaderItem(1,header1);

    playlistModel->setHeaderData(0, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);
    playlistModel->setHeaderData(1, Qt::Horizontal, Qt::AlignLeft, Qt::TextAlignmentRole);

    ui->playlistView->verticalHeader()->hide();

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_button_start_stream_clicked
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: on_button_start_stream_clicked()
--
-- NOTES: Logic to handle start stream button click.
----------------------------------------------------------------------------------------------------------------------*/
void server::on_button_start_stream_clicked()
{
    if(!isPlaying && !isConnected){
        //DWORD id;
        play = new playerManager();
        connect(play, SIGNAL(relayCurrentSong(QString)), this, SLOT(updateCurrentlyPlayingLabel(QString)), Qt::UniqueConnection);

        //check if the playlist is empty
        if(playlistWithPath.isEmpty()){
            networkutility::debugMessage("playlist empty");
            return;
        }
        //pass in the file along with filepath to startSong
        updateCurrentlyPlayingLabel(playlist.at(0));
        ui->label_server_stream_status->setText("Status: Streaming");
        ui->label_server_stream_status->setStyleSheet("#label_server_stream_status { color: #72FFF7; }");
        play->startSong(playlistWithPath.at(0));
        isPlaying = true;
        isConnected = true;
    } else {
        if(isPlaying){
            play->endPlayer();
            ui->label_server_stream_status->setText("Status: Stopped");
            ui->label_server_stream_status->setStyleSheet("#label_server_stream_status { color: #E4144C; }");

            isPlaying = false;
        }else{
            play->restartStream();
            ui->label_server_stream_status->setText("Status: Streaming");
            ui->label_server_stream_status->setStyleSheet("#label_server_stream_status { color: #72FFF7; }");

            isPlaying = true;
        }


    }



    //loveQt = CreateThread(NULL, 0, doServer, (void *)0, 0, &id);
   // player.startPlayer();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: updateCurrentlyPlayingLabel
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: updateCurrentlyPlayingLabel(QString songName)
--
-- NOTES: Updates the GUI label to show the currently playing song.
----------------------------------------------------------------------------------------------------------------------*/
void server::updateCurrentlyPlayingLabel(QString songName){
    ui->label_server_stream_current->setText("Currently playing: " + songName);
}
