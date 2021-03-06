#include "client.h"
#include "globals.h"
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: client.cpp
--
-- FUNCTIONS:
-- client::client(QWidget *parent) : QWidget(parent), ui(new Ui::client)
-- client::~client()
-- void client::on_connectButton_clicked()
-- void client::on_disconnectButton_clicked()
-- void client::on_updateSongButton_clicked()
-- void client::setDownloadStatus(int state)
-- void client::setUploadStatus(int state)
-- void client::toggleInput(bool state)
-- void client::updateUsers(QVector<QString> userList)
-- void client::updateCallLabel(QString caller)
-- void client::handleStateChanged(QAudio::State newState)
-- void client::on_updateVoiceUsersButton_clicked()
-- void client::on_playStreamButton_clicked()
-- std::vector<int> client::getTime(int time)
-- void client::updateSlider(float percent, int songTime)
-- void client::on_stopStreamButton_clicked()
-- void client::on_rewindStreamButton_clicked()
-- void client::handleUpdateStatusBar(bool connected)
-- void client::on_horizontalSlider_2_sliderReleased()
-- void client::on_liveStreamButton_clicked()
-- void client::on_horizontalSlider_2_sliderPressed()
-- void client::on_updateStreamPlaylist_clicked()
-- void client::setCurrentlyPlaying(QString songName)
-- void client::on_voiceChatButton_clicked()
-- void client::on_endChatButton_clicked()
-- void client::on_acceptVoiceButton_clicked()
-- void client::tabSelected()
-- void client::on_button_uploadDirectory_clicked()
-- void client::on_volumeSlider_valueChanged(int value)
-- void client::on_connectedWidget_itemSelectionChanged()
-- void client::on_pushButton_12_clicked()
-- void client::on_downloadFileWidget_itemSelectionChanged()
-- void client::on_uploadFileWidget_itemSelectionChanged()
-- void client::updateIncomingVoiceChatText(QString name)
-- void client::on_streamingPlaylistWidget_itemSelectionChanged()
-- void client::toggleVoiceChatAcceptRejectButtons(bool state)
-- void client::on_rejectVoiceButton_clicked()
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
--
-- NOTES: A circular buffer to store char array data. Provides functions to add and remove data.
----------------------------------------------------------------------------------------------------------------------*/
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
bool voted = false;
client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    LPCWSTR idk = L"Astring";
    DWORD id;
    dataInBuffer = CreateEvent(NULL, TRUE, FALSE, idk);
    streamStop = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)L"work");
    paused = true;
    streamSetup = false;
    ui->setupUi(this);

    ui->uploadButton->setEnabled(false);
    ui->downloadSongButton->setEnabled(false);
    ui->disconnectButton->setEnabled(false);
    ui->voiceChatButton->setEnabled(false);
    ui->endChatButton->setEnabled(false);
    toggleVoiceChatAcceptRejectButtons(false);

    for(int i= 1; i < 5; i++)
    {
        ui->tabWidget->setTabEnabled(i, false);
    }
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()), Qt::UniqueConnection);
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

    mUsername = username;

    //connect
    receiveTCPThread = new QThread;
    receiveTCPWorker = new ThreadManager();


    fileThread = new QThread;
    fileWorker = new ThreadManager();

    receiveVoiceChatThread = new QThread;
    receiveVoiceChatWorker = new ThreadManager();

    streamUDPThread = new QThread;
    streamUDPWorker = new UDPThreadManager();

    receiveTCPWorker->moveToThread(receiveTCPThread);
    receiveVoiceChatWorker->moveToThread(receiveVoiceChatThread);
    streamUDPWorker->moveToThread(streamUDPThread);
    fileWorker->moveToThread(fileThread);

    //Not sure why this is done, but its something to do with passing objects in threads.
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    qRegisterMetaType<SOCKET>("SOCKET");
    qRegisterMetaType<sockaddr_in>("sockaddr_in");

    connect(receiveTCPWorker, SIGNAL(signalConnect(QString, QString, QString)), receiveTCPWorker, SLOT(connect(QString, QString, QString)), Qt::UniqueConnection);
    connect(receiveTCPWorker, SIGNAL(updateUserList(QVector<QString>)), this, SLOT(updateUsers(QVector<QString>)), Qt::UniqueConnection);
    connect(receiveTCPWorker, SIGNAL(updateSongList(QVector<QString>)), this, SLOT(updateSongs(QVector<QString>)), Qt::UniqueConnection);
    connect(receiveTCPWorker, SIGNAL(signalHandleRequest()), receiveTCPWorker, SLOT(handleRequest()), Qt::UniqueConnection);
    connect(receiveTCPWorker, SIGNAL(updateStatusBar(bool)), this, SLOT(handleUpdateStatusBar(bool)), Qt::UniqueConnection);
    connect(receiveTCPWorker, SIGNAL(finished()), this, SLOT(quit()), Qt::UniqueConnection);

    //connect(sendTCPWorker, SIGNAL(finished()), sendTCPThread, SLOT(quit()), Qt::UniqueConnection);
    connect(receiveVoiceChatWorker, SIGNAL(signalUpdateVoiceChatUser(QString)), this, SLOT(updateIncomingVoiceChatText(QString)), Qt::UniqueConnection);
    connect(receiveVoiceChatWorker, SIGNAL(signalToggleVoiceButtons(bool)), this, SLOT(toggleVoiceChatAcceptRejectButtons(bool)), Qt::UniqueConnection);
    connect(receiveVoiceChatWorker, SIGNAL(signalVoiceChat()), receiveVoiceChatWorker, SLOT(setupVoiceChat()), Qt::UniqueConnection);
    connect(receiveVoiceChatWorker, SIGNAL(updateCaller(QString)), this, SLOT(updateCallLabel(QString)), Qt::UniqueConnection);
    connect(receiveVoiceChatWorker, SIGNAL(finished()), receiveTCPThread, SLOT(quit()), Qt::UniqueConnection);

    connect(streamUDPWorker, SIGNAL(signalUDPWorker(SOCKET, struct sockaddr_in)), streamUDPWorker, SLOT(UDPWorker(SOCKET, struct sockaddr_in)), Qt::UniqueConnection);
    qDebug() << "UDPWorker connected";

    connect(receiveTCPWorker, SIGNAL(signalStartPlayer()), &rec, SLOT(startPlayer()), Qt::UniqueConnection);

    receiveTCPThread->start();
    receiveVoiceChatThread->start();
    fileThread->start();
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
    connect(receiveTCPWorker, SIGNAL(signalDisconnect()), receiveTCPWorker, SLOT(disconnect()), Qt::UniqueConnection);
    connect(receiveVoiceChatWorker, SIGNAL(signalDisconnect()), receiveVoiceChatWorker, SLOT(disconnect()), Qt::UniqueConnection);

    //disconnect
    emit receiveTCPWorker->signalDisconnect();
    emit receiveVoiceChatWorker->signalDisconnect();

    closesocket(TCPSocket);
    closesocket(VCSocket);
    closesocket(AcceptSocket);
    closesocket(StreamSocket);

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
    connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()), Qt::UniqueConnection);
    emit receiveTCPWorker->signalSongRefresh();
}

void client::on_uploadButton_clicked()
{
    connect(receiveTCPWorker, SIGNAL(signalUpload(QString)), receiveTCPWorker, SLOT(SendUploadRequest(QString)), Qt::UniqueConnection);
    connect(receiveTCPWorker, SIGNAL(signalUploadStatus(int)), this, SLOT(setUploadStatus(int)), Qt::UniqueConnection);
    QString songName = ui->uploadFileWidget->currentItem()->text();
    emit receiveTCPWorker->signalUploadStatus(1);

    //find the corresponding song name from the path list

    emit receiveTCPWorker->signalUpload(songName);
}
void client::on_downloadSongButton_clicked()
{
    connect(fileWorker, SIGNAL(signalDownload(QString)), fileWorker, SLOT(SendDownloadRequest(QString)), Qt::UniqueConnection);
    connect(fileWorker, SIGNAL(signalDownloadStatus(int)), this, SLOT(setDownloadStatus(int)), Qt::UniqueConnection);
    QString songName = QString("%1%2").arg(REQ_DOWNLOAD).arg(ui->downloadFileWidget->currentItem()->text());
    emit fileWorker->signalDownload(songName);
    emit fileWorker->signalDownloadStatus(1);
}
void client::setDownloadStatus(int state){

    switch(state){
    case 0:
        ui->label_downloadStatus->setText("");
        break;
    case 1:
        ui->label_downloadStatus->setText("Downloading...");
        break;
    case 2:
        ui->label_downloadStatus->setText("Download complete");
        break;
    }
}
void client::setUploadStatus(int state){

    switch(state){
    case 0:
        ui->label_uploadStatus->setText("");
        break;
    case 1:
        ui->label_uploadStatus->setText("Uploading...");
        break;
    case 2:
        ui->label_uploadStatus->setText("Upload complete");
        break;
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
    connect(receiveTCPWorker, SIGNAL(signalVoiceRefresh()), receiveTCPWorker, SLOT(SendVoiceRefreshRequest()), Qt::UniqueConnection);
    emit receiveTCPWorker->signalVoiceRefresh();
}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_playStremButton_clicked()
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	on_playStreamButton_clicked
--
--
-- RETURNS: VOID
-- NOTES: Starts the audio streaming
----------------------------------------------------------------------------------------------------------------------*/
void client::on_playStreamButton_clicked()
{
    if(!streamSetup){
        streamUDPThread->start();
        qDebug() << "Play Stream Button is clicked";
        //Not sure why this is done, but its something to do with passing objects in threads.
        connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()), Qt::UniqueConnection);
        //emit receiveTCPWorker->signalSongRefresh();
        connect(&play, SIGNAL(updateCurrentlyPlaying(QString)), this, SLOT(setCurrentlyPlaying(QString)), Qt::UniqueConnection);

        streamUDPWorker->initMultiCastSock();

        //Check if StreamSocket socket is not null
        if (StreamSocket == 0)
        {
            qDebug() << "Stream Socket is null";
            return;
        }

        qDebug() << "Starting to listen";
        WaitForSingleObject(dataInBuffer, 5000);
        ResetEvent(dataInBuffer);
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

    //disable other tabs if streaming started
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getTime
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	getTime(int time)
--
--
-- RETURNS: VETOR of two integers - minutes and seconds
-- NOTES: Call to convert integer into minute sand seconds
----------------------------------------------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	updateSlider
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	updateSlider(float percent, int songTime)
--                      float percent - percent of the song completed - move the bar this far
--                      songTime - total song time
-- RETURNS:
-- NOTES:
----------------------------------------------------------------------------------------------------------------------*/
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

    connect(streamUDPWorker, SIGNAL(signalDisconnect()), streamUDPWorker, SLOT(disconnect()), Qt::UniqueConnection);
    closesocket(StreamSocket);
    closesocket(SI->Socket);
    emit streamUDPWorker->disconnect();

    streamSetup = false;
    paused = true;
    cData.clear();
    ResetEvent(dataInBuffer);
    SetEvent(streamStop);

    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->tabWidget->setTabEnabled(3, true);

    qDebug() << "After Disconnet";
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	rewindStreamButton_clicked
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	on_rewindStreamButton_clicked
--
--
-- RETURNS: VOID
-- NOTES: rewind the stream as far as possible
----------------------------------------------------------------------------------------------------------------------*/
void client::on_rewindStreamButton_clicked()
{
    play.rewind();
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


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	on_horizontalSlider_2_sliderReleased
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	on_horizontalSlider_2_sliderReleased
--
--
-- RETURNS:VOID
-- NOTES: Called when the scroll bar is moved, updates the song position
----------------------------------------------------------------------------------------------------------------------*/
void client::on_horizontalSlider_2_sliderReleased()
{
    drag = false;
    play.sliderChange(ui->horizontalSlider_2->value());
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	on_liveStreamButton_clicked
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	on_liveStreamButton_clicke
--
--
-- RETURNS: VOID
-- NOTES: moves the stream to the current head of the buffer -- to live time
----------------------------------------------------------------------------------------------------------------------*/
void client::on_liveStreamButton_clicked()
{

    play.jumpLive();
}

void client::on_horizontalSlider_2_sliderPressed()
{
    drag = true;
}
void client::on_updateStreamPlaylist_clicked(){
    connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()), Qt::UniqueConnection);
    emit receiveTCPWorker->signalSongRefresh();
}

void client::setCurrentlyPlaying(QString songName){
    voted = false;
    ui->currentlyPlayingText->setText(songName);
    ui->pushButton_12->setEnabled(true);
    ui->volumeSlider->valueChanged(ui->volumeSlider->value());
    ui->label_selectedSongNameVote->setText("No song voted for yet.");
}
void client::on_voiceChatButton_clicked()
{
    char buf[PACKET_LEN];
    char *clientIP = buf;
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(4, false);
    //ask the server for the IP corresponding to the selected username
    connect(receiveTCPWorker, SIGNAL(signalGetVoiceChatIP(QString)), receiveTCPWorker, SLOT(requestVoiceChatIP(QString)), Qt::UniqueConnection);
    QString username = QString("%1%2").arg(REQ_CHAT_IP).arg(ui->connectedWidget->currentItem()->text());

    //check if the user is trying to call himself
    if(QString::compare(username,mUsername,Qt::CaseSensitive) == 0){
        return;
    }

    emit receiveTCPWorker->signalGetVoiceChatIP(username);

    //read the ip from the server
    if(receiveTCP(TCPSocket, clientIP)){
        qDebug() << "got a client IP back?";
        qDebug() << clientIP;
    }

    connect(receiveTCPWorker, SIGNAL(signalVoiceConnect(QString)), receiveTCPWorker, SLOT(VoiceConnect(QString)), Qt::UniqueConnection);
    emit receiveTCPWorker->signalVoiceConnect(clientIP);

    rec.initializeAudio();
    rec.startPlayer();

    toggleVoiceChatAcceptRejectButtons(false);
    ui->voiceChatButton->setEnabled(false);
    ui->label_callStatus_2->setText("Connected");
    ui->endChatButton->setEnabled(true);
}
void client::on_endChatButton_clicked()
{
    shutdown(VCRecieveSocket, SD_BOTH);
    shutdown(VCSendSocket, SD_BOTH);
    shutdown(VCConnectSocket, SD_BOTH);
    shutdown(AcceptSocket, SD_BOTH);
    shutdown(VCSocket, SD_BOTH);

    closesocket(VCRecieveSocket);
    closesocket(VCSendSocket);
    closesocket(VCConnectSocket);
    qDebug() << "Closing Sockets!";
    connectionRequested = false;

    emit receiveVoiceChatWorker->signalDisconnect();

    if (rec.player != NULL)
    {
        rec.stopRecording();
        cData.clear();
    }

    ui->tabWidget->setTabEnabled(0, true);
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
    ui->tabWidget->setTabEnabled(4, true);
    ui->label_callStatus_2->setText("Not connected");
    ui->voiceCallLabel->setText("");
    ui->endChatButton->setEnabled(false);

    QModelIndex item = ui->connectedWidget->currentIndex();
    if(item.row() != -1){
        ui->voiceChatButton->setEnabled(true);
    }
    emit receiveVoiceChatWorker->signalVoiceChat();
}
void client::on_acceptVoiceButton_clicked()
{
    qDebug() << "ON ACCEPT BUTTON";
    rec.initializeAudio();
    rec.startPlayer();

    //disable other tabs when voice chat started
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(4, false);

    ui->label_callStatus_2->setText("Connected");
    ui->endChatButton->setEnabled(true);
    toggleVoiceChatAcceptRejectButtons(false);
}

void client::tabSelected(){
    //file download tab
    if(ui->tabWidget->currentIndex()==1){
        connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()), Qt::UniqueConnection);
        emit receiveTCPWorker->signalSongRefresh();

    }

    //voice chat tab
    if(ui->tabWidget->currentIndex()==3){
        connect(receiveTCPWorker, SIGNAL(signalVoiceRefresh()), receiveTCPWorker, SLOT(SendVoiceRefreshRequest()), Qt::UniqueConnection);
        emit receiveTCPWorker->SendVoiceRefreshRequest();
    }

    //stream tab
    if(ui->tabWidget->currentIndex()==4){
        connect(receiveTCPWorker, SIGNAL(signalSongRefresh()), receiveTCPWorker, SLOT(SendSongRefreshRequest()), Qt::UniqueConnection);
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	on_volumeSlider_valueChanged(int value)
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	on_volumeSlider_valueChanged(int value)
--
--
-- RETURNS: VOID
-- NOTES: updates the audio player volume on slider change
----------------------------------------------------------------------------------------------------------------------*/
void client::on_volumeSlider_valueChanged(int value)
{
    play.updateVolume((float)(value / 100.0f));
}

void client::on_connectedWidget_itemSelectionChanged()
{    
    QString tempName = ui->connectedWidget->currentItem()->text();
    if(QString::compare(tempName,mUsername,Qt::CaseSensitive) == 0){
        ui->label_selectedUserName->setText("You can't call yourself!");
        ui->voiceChatButton->setEnabled(false);
        return;
    }
    ui->label_selectedUserName->setText(ui->connectedWidget->currentItem()->text());
    ui->voiceChatButton->setEnabled(true);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	on_pushButton_12_clicked
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	on_pushButton_12_clicked
--
--
-- RETURNS: VOID
-- NOTES: Song vote button - sends your vote to the server
----------------------------------------------------------------------------------------------------------------------*/
void client::on_pushButton_12_clicked()
{
    int vote = ui->streamingPlaylistWidget->currentRow();
    if(vote == -1)
        return;

    char send[1024] = { 0 };
    char * sVote = send;
    sprintf(send, ")%d", vote);
    connect(receiveTCPWorker, SIGNAL(songVote(char *)), receiveTCPWorker, SLOT(voteForSong(char *)));
    emit receiveTCPWorker->songVote((char *)sVote);
    ui->pushButton_12->setEnabled(false);
    voted = true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	void on_downloadFileWidget_itemSelectionChanged
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin
-- INTERFACE:  void on_downloadFileWidget_itemSelectionChanged()
--
--
-- RETURNS:
-- NOTES: Unlocks download file button on listwidget item select
----------------------------------------------------------------------------------------------------------------------*/
void client::on_downloadFileWidget_itemSelectionChanged()
{
    setDownloadStatus(0);
    ui->downloadSongButton->setEnabled(true);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	on_uploadFileWidget_itemSelectionChanged
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin
-- INTERFACE:	on_uploadFileWidget_itemSelectionChanged()
--
--
-- RETURNS: VOID
-- NOTES: Unlocks upload file button when an item is selected from the qlistwidget
----------------------------------------------------------------------------------------------------------------------*/
void client::on_uploadFileWidget_itemSelectionChanged()
{
    setUploadStatus(0);
    ui->uploadButton->setEnabled(true);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	updateIncomingVoiceChatText
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin
-- INTERFACE:	updateIncomingVoiceChatText(Qstring name)
--
--
-- RETURNS: VOID
-- NOTES: Updates the call incoming from dialogue
----------------------------------------------------------------------------------------------------------------------*/
void client::updateIncomingVoiceChatText(QString name){
    QString temp = "Incoming call from ";
    temp.append(name);
    ui->voiceCallLabel->setText(temp);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	on_streamingPlaylistWidget_itemSelectionChanged
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin
-- INTERFACE:	on_streamingPlaylistWidget_itemSelectionChanged()
--
--
-- RETURNS: VOID
-- NOTES: Changes label text to match list widget selection
----------------------------------------------------------------------------------------------------------------------*/
void client::on_streamingPlaylistWidget_itemSelectionChanged()
{
    if(!voted)
        ui->label_selectedSongNameVote->setText(ui->streamingPlaylistWidget->currentItem()->text());
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	toggleVoiceChatAcceptRejectButtons
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin
-- INTERFACE:	toggleVoiceChatAcceptRejectButtons(bool state)
--                          bool state - enabled/disabled
--
-- RETURNS: VOID
-- NOTES: Sets the status of the voice chat butotns - enabled/disabled
----------------------------------------------------------------------------------------------------------------------*/
void client::toggleVoiceChatAcceptRejectButtons(bool state){
    ui->acceptVoiceButton->setEnabled(state);
    ui->rejectVoiceButton->setEnabled(state);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: on_rejectVoiceButton_clicked()
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin
-- INTERFACE:	on_rejectVoiceButton_clicked()
--
--
-- RETURNS: VOID
-- NOTES: Rejects a voice call -> resets the text and buttons
----------------------------------------------------------------------------------------------------------------------*/
void client::on_rejectVoiceButton_clicked()
{
    ui->voiceCallLabel->setText("");
    ui->voiceChatButton->setEnabled(true);
}
