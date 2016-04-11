#include "client.h"
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
    ui->setupUi(this);
    for(int i= 1; i < 5; i++)
    {
        ui->tabWidget->setTabEnabled(i, false);
    }
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
    connect(receiveTCPWorker, SIGNAL(finished()), receiveVoiceChatThread, SLOT(quit()));

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
    //emit receiveWorker->signalUpload();
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
    }
    printf("State changed");
    fflush(stdout);
}

void client::on_updateVoiceUsersButton_clicked()
{
    qDebug() << "Send Refresh Button is clicked";
    connect(receiveTCPWorker, SIGNAL(signalVoiceRefresh()), receiveTCPWorker, SLOT(SendVoiceRefreshRequest()));
    emit receiveTCPWorker->SendVoiceRefreshRequest();
}

void client::on_playStreamButton_clicked()
{
    //PLAY
    printf("I hate alvin");
	qDebug() << "Play Stream Button is clicked";
    //Not sure why this is done, but its something to do with passing objects in threads.
    connect(&play, SIGNAL(updateCurrentlyPlaying(QString)), this, SLOT(setCurrentlyPlaying(QString)));
    streamUDPWorker->initMultiCastSock();

	//Check if StreamSocket socket is not null
	if (StreamSocket == 0)
		return;

	qDebug() << "Starting to listen";
    play.setSocket(StreamSocket);
}
void client::updateSlider(int percent){
    if(drag)
        return;
    ui->horizontalSlider_2->setSliderPosition(percent);
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
        ui->label_statusbar_text->setText("Status: CONNECTED");
        ui->label_statusbar_text->setStyleSheet("#label_statusbar_text { color: #78ff00; }");
    } else {
        ui->label_statusbar_text->setText("Status: DISCONNECTED");
        ui->label_statusbar_text->setStyleSheet("#label_statusbar_text { color: #ff104e; }");
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
}
void client::on_voiceChatButton_clicked()
{
    rec.initializeAudio();
    rec.startPlayer();
    return;
    connect(receiveTCPWorker, SIGNAL(signalVoiceConnect()), receiveTCPWorker, SLOT(VoiceConnect()));
    emit receiveTCPWorker->signalVoiceConnect();
    qDebug() << "Before init";
    rec.initializeAudio();
    qDebug() << "after init";
    rec.startPlayer();
}
void client::on_endChatButton_clicked()
{
    //emit signalStopRecording();
    rec.stopRecording();
}

void client::on_acceptVoiceButton_clicked()
{
    cData.tail = cData.headBuff;
}
