#include "client.h"
#include "globals.h"
circlebuff music;
QFile sourceFile;
QBuffer playBuffer;
myBuffer play;
QAudioOutput * testPlayer;
QFile testFile;
char musicBuff[20000] = { 0 };
extern QObject * mw;

extern QObject * bf;
bool drag = false;
client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    LPCWSTR idk = L"Astring";
    DWORD id;
    dataInBuffer = CreateEvent(NULL, TRUE, FALSE, idk);
    paused = true;
    streamSetup = false;
    ui->setupUi(this);

    ui->disconnectButton->setEnabled(false);
    for(int i= 1; i < 5; i++)
    {
        ui->tabWidget->setTabEnabled(i, false);
    }
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
    mw = this;
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
    receiveTCPWorker = new ThreadManager();

    receiveVoiceChatThread = new QThread;
    receiveVoiceChatWorker = new ThreadManager();

    streamUDPThread = new QThread;
    streamUDPWorker = new UDPThreadManager();

    receiveTCPWorker->moveToThread(receiveTCPThread);
    receiveVoiceChatWorker->moveToThread(receiveVoiceChatThread);
    streamUDPWorker->moveToThread(streamUDPThread);

    //Not sure why this is done, but its something to do with passing objects in threads.
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    qRegisterMetaType<SOCKET>("SOCKET");
    qRegisterMetaType<sockaddr_in>("sockaddr_in");

    connect(receiveTCPWorker, SIGNAL(signalConnect(QString, QString, QString)), receiveTCPWorker, SLOT(connect(QString, QString, QString)));
    connect(receiveTCPWorker, SIGNAL(updateUserList(QVector<QString>)), this, SLOT(updateUsers(QVector<QString>)));
    connect(receiveTCPWorker, SIGNAL(updateSongList(QVector<QString>)), this, SLOT(updateSongs(QVector<QString>)));
    connect(receiveTCPWorker, SIGNAL(signalHandleRequest()), receiveTCPWorker, SLOT(handleRequest()));
    connect(receiveTCPWorker, SIGNAL(updateStatusBar(bool)), this, SLOT(handleUpdateStatusBar(bool)));
    connect(receiveTCPWorker, SIGNAL(finished()), this, SLOT(callNotification()));
    connect(receiveTCPWorker, SIGNAL(signalCallNotification()), receiveVoiceChatThread, SLOT(quit()));

    //connect(sendTCPWorker, SIGNAL(finished()), sendTCPThread, SLOT(quit()));

    connect(receiveVoiceChatWorker, SIGNAL(signalVoiceChat()), receiveVoiceChatWorker, SLOT(setupVoiceChat()));
    connect(receiveVoiceChatWorker, SIGNAL(updateCaller(QString)), this, SLOT(updateCallLabel(QString)));
    connect(receiveVoiceChatWorker, SIGNAL(finished()), receiveTCPThread, SLOT(quit()));

    connect(streamUDPWorker, SIGNAL(signalUDPWorker(SOCKET, struct sockaddr_in)), streamUDPWorker, SLOT(UDPWorker(SOCKET, struct sockaddr_in)));
    qDebug() << "UDPWorker connected";

    connect(receiveTCPWorker, SIGNAL(signalStartPlayer()), &rec, SLOT(startPlayer()));

    receiveTCPThread->start();
    receiveVoiceChatThread->start();
    streamUDPThread->start();

    emit receiveTCPWorker->signalConnect(ipaddr, portnum, username);

    client::toggleInput(false);
    ui->connectStatus->setText("Connected");
    ui->disconnectButton->setEnabled(true);
    ui->connectButton->setEnabled(false);
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, true);
    }
    emit receiveVoiceChatWorker->signalVoiceChat();
}

void client::on_disconnectButton_clicked()
{
    connect(receiveTCPWorker, SIGNAL(signalDisconnect()), receiveTCPWorker, SLOT(disconnect()));
    connect(receiveVoiceChatWorker, SIGNAL(signalDisconnect()), receiveVoiceChatWorker, SLOT(disconnect()));

    //disconnect
    emit receiveTCPWorker->signalDisconnect();
    emit receiveVoiceChatWorker->signalDisconnect();

    closesocket(TCPSocket);
    closesocket(VCSocket);
    WSACleanup();

    client::toggleInput(true);
    ui->connectStatus->setText("Disconnected");
    ui->disconnectButton->setEnabled(false);
    ui->connectButton->setEnabled(true);
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, false);
    }
}
void client::on_updateSongButton_clicked()
{
    qDebug() << "Send Refresh Button is clicked";
    connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()));
    emit receiveTCPWorker->signalSongRefresh();
}

void client::on_uploadButton_clicked()
{
    connect(receiveTCPWorker, SIGNAL(signalUpload(QString)), receiveTCPWorker, SLOT(SendUploadRequest(QString)));
    QString songName = ui->uploadFileWidget->currentItem()->text();

    //find the corresponding song name from the path list

    emit receiveTCPWorker->signalUpload(songName);
}
void client::on_downloadSongButton_clicked()
{
    connect(receiveTCPWorker, SIGNAL(signalDownload(QString)), receiveTCPWorker, SLOT(SendDownloadRequest(QString)));
    QString songName = QString("%1%2").arg(REQ_DOWNLOAD).arg(ui->downloadFileWidget->currentItem()->text());
    emit receiveTCPWorker->signalDownload(songName);
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
void client::updateSongs(QVector<QString> playList)
{
    //update the download tab
    ui->downloadFileWidget->clear();
    for(auto& user : playList){
		ui->downloadFileWidget->addItem(user);
    }

    //update the streaming tab
    ui->streamingPlaylistWidget->clear();
    for(auto& user : playList){
        ui->streamingPlaylistWidget->addItem(user);
    }
}
void client::updateCallLabel(QString caller)
{
    if (caller == NULL || caller == "")
        return;
    ui->voiceCallLabel->setText("From: " + caller);
}
void client::handleStateChanged(QAudio::State newState){
    switch(newState){
    case QAudio::IdleState:
		printf("Audio in idle");
		fflush(stdout);

		break;

    case QAudio::StoppedState:
        printf("Audio is stopped");
        fflush(stdout);
        break;
    default:
        break;
    }
    printf("State changed");
    fflush(stdout);
}

void client::on_updateVoiceUsersButton_clicked()
{
    qDebug() << "Send Refresh Button is clicked";
    connect(receiveTCPWorker, SIGNAL(signalVoiceRefresh()), receiveTCPWorker, SLOT(SendVoiceRefreshRequest()));
    emit receiveTCPWorker->signalVoiceRefresh();
}

void client::on_playStreamButton_clicked()
{
    if(!streamSetup){
        qDebug() << "Play Stream Button is clicked";
        //Not sure why this is done, but its something to do with passing objects in threads.
        connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()));
        emit receiveTCPWorker->signalSongRefresh();
        connect(&play, SIGNAL(updateCurrentlyPlaying(QString)), this, SLOT(setCurrentlyPlaying(QString)));

        streamUDPWorker->initMultiCastSock();

        //Check if StreamSocket socket is not null
        if (StreamSocket == 0)
            return;

        qDebug() << "Starting to listen";
        WaitForSingleObject(dataInBuffer, 5000);
        play.setSocket(StreamSocket);
        streamSetup = true;
    } else {
        if(paused){

            paused = false;
            play.pausePlayer();
        }
        else{
            play.resumePlayer();
            paused = true;
        }
    }

}
std::vector<int> client::getTime(int time){
    std::vector<int> ret;
    int h, m;
    m = time % 60;
    time -= m;
    h = time / 60;
    ret.push_back(h);
    ret.push_back(m);
    return ret;
}

void client::updateSlider(float percent, int songTime){
    if(drag)
        return;
    ui->horizontalSlider_2->setSliderPosition((int)percent);
    std::vector<int> time = getTime(songTime);
    std::vector<int> cur = getTime(songTime * percent / 300);
    QString rem, current;
    if(time.at(1) < 10){
        rem = QString("%1:0%2").arg(time.at(0)).arg(time.at(1));
    }
    else{
        rem = QString("%1:%2").arg(time.at(0)).arg(time.at(1));
    }
    if(cur.at(1) < 10){
        current = QString("%1:0%2").arg(cur.at(0)).arg(cur.at(1));
    }
    else{
        current = QString("%1:%2").arg(cur.at(0)).arg(cur.at(1));

    }

    ui->label_songRemainingTime->setText(rem);
    ui->label_songElapsedTime->setText(current);



}

void client::on_stopStreamButton_clicked()
{
    //STOP
    connect(streamUDPWorker, SIGNAL(signalDisconnect()), streamUDPWorker, SLOT(disconnect()));
    closesocket(StreamSocket);
    closesocket(SI->Socket);
    emit streamUDPWorker->disconnect();

    qDebug() << "After Disconnet";
}

void client::on_rewindStreamButton_clicked()
{
    //REWIND
    cData.tail -= 20;
    if(cData.tail < 0)
        cData.tail = 0;

}

void client::handleUpdateStatusBar(bool connected){
    if(connected){
        ui->label_statusbar_text->setText("Status: Connected");
        ui->label_statusbar_text->setStyleSheet("#label_statusbar_text { color: #72FFF7; }");
    } else {
        ui->label_statusbar_text->setText("Status: Disconnected");
        ui->label_statusbar_text->setStyleSheet("#label_statusbar_text { color: #E4144C; }");
    }
}


void client::on_horizontalSlider_2_sliderReleased()
{
    drag = false;
    play.sliderChange(ui->horizontalSlider_2->value());
}

void client::on_liveStreamButton_clicked()
{

    play.jumpLive();
}

void client::on_horizontalSlider_2_sliderPressed()
{
    drag = true;
}
void client::on_updateStreamPlaylist_clicked(){
    connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()));
    emit receiveTCPWorker->signalSongRefresh();
}

void client::setCurrentlyPlaying(QString songName){
    ui->currentlyPlayingText->setText(songName);
    ui->pushButton_12->setEnabled(true);
}
void client::on_voiceChatButton_clicked()
{
    char buf[PACKET_LEN];
    char *clientIP = buf;

    //ask the server for the IP corresponding to the selected username
    connect(receiveTCPWorker, SIGNAL(signalGetVoiceChatIP(QString)), receiveTCPWorker, SLOT(requestVoiceChatIP(QString)));
    QString username = QString("%1%2").arg(REQ_CHAT_IP).arg(ui->connectedWidget->currentItem()->text());
    emit receiveTCPWorker->signalGetVoiceChatIP(username);

    //read the ip from the server
    if(receiveTCP(TCPSocket, clientIP)){
        qDebug() << "got a client IP back?";
        qDebug() << clientIP;
    }

    connect(receiveTCPWorker, SIGNAL(signalVoiceConnect(QString)), receiveTCPWorker, SLOT(VoiceConnect(QString)));
    emit receiveTCPWorker->signalVoiceConnect(clientIP);

    rec.initializeAudio();

    rec.startPlayer();
    //rec.startSecondary();
}
void client::on_endChatButton_clicked()
{
    closesocket(VCRecieveSocket);
    closesocket(VCSendSocket);
    closesocket(VCConnectSocket);

    emit receiveTCPWorker->signalDisconnect();
    if (rec.player != NULL)
        rec.stopRecording();
}


void client::on_acceptVoiceButton_clicked()
{
    qDebug() << "ON ACCEPT BUTTON";
    rec.initializeAudio();
    rec.startPlayer();
}

void client::tabSelected(){
    //file download tab
    if(ui->tabWidget->currentIndex()==1){
        connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()));
        emit receiveTCPWorker->signalSongRefresh();
    }

    //voice chat tab
    if(ui->tabWidget->currentIndex()==3){
        connect(receiveTCPWorker, SIGNAL(signalVoiceRefresh()), receiveTCPWorker, SLOT(SendVoiceRefreshRequest()));
        emit receiveTCPWorker->SendVoiceRefreshRequest();
    }

    //stream tab
    if(ui->tabWidget->currentIndex()==4){
        connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()));
        emit receiveTCPWorker->signalSongRefresh();
    }
}

void client::on_button_uploadDirectory_clicked()
{
    QFileDialog selectUploadDirectoryDialog(this);
    selectUploadDirectoryDialog.setFileMode(QFileDialog::Directory);

    QString uploadDirectory;
    uploadDirectory = selectUploadDirectoryDialog.getExistingDirectory(this, tr("Select Upload Directory"), QDir::currentPath(), QFileDialog::ShowDirsOnly);
    ui->upload_directory_text->setText(uploadDirectory);

    QDir *dir = new QDir(uploadDirectory);
    dir->setNameFilters(QStringList("*.wav"));
    dir->setFilter(QDir::Files);

    //update the uploadList with the files in the selected directory
    uploadList.clear();
    QStringList fileList = dir->entryList();
    for (int i = 0; i < fileList.count(); i++){
        qDebug() << fileList[i];
        uploadList.push_back(fileList[i]);
        uploadListWithPath.push_back(uploadDirectory + "/" + fileList[i]);
    }

    //update the upload list widget
    ui->uploadFileWidget->clear();
    for(auto& file : uploadList){
        ui->uploadFileWidget->addItem(file);
    }
}

void client::on_volumeSlider_valueChanged(int value)
{
    play.updateVolume((float)(value / 100.0f));
}

void client::on_connectedWidget_itemSelectionChanged()
{
    ui->label_selectedUserName->setText(ui->connectedWidget->currentItem()->text());
}
void client::callNotification()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Call Incoming!", "Quit?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        qDebug() << "Yes was clicked";
    } else
    {
        qDebug() << "Yes was *not* clicked";
    }
}

void client::on_pushButton_12_clicked()
{
  /* QList<QListWidgetItem *>  sel = ui->streamingPlaylistWidget->selectedItems();
   if(sel.size() == 0){
       return;
   }
   else{
   }*/

    int vote = ui->streamingPlaylistWidget->currentRow();
    if(vote == -1)
        return;

    char send[1024] = { 0 };
    char * sVote = send;
    sprintf(send, ")%d", vote);
    connect(receiveTCPWorker, SIGNAL(songVote(char *)), receiveTCPWorker, SLOT(voteForSong(char *)));
    emit receiveTCPWorker->songVote((char *)sVote);
    ui->pushButton_12->setEnabled(false);

}
