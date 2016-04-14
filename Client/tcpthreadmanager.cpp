#include "tcpthreadmanager.h"

HANDLE newData;
HANDLE readDone;
HANDLE fileDone;
circlebuff c;

ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{
    TCPSocket = 0;
    VCSocket = 0;
    StreamSocket = 0;
}


ThreadManager::~ThreadManager()
{
    /*
    closesocket(TCPSocket);
    TCPSocket = 0;
    VCSocket = 0;
    WSACleanup();
    */
}


void ThreadManager::connect(QString ipaddr, QString portnum, QString username)
{
    mUsername = username;
    qDebug() << "Connect Thread Created";

    if(TCPSocket != 0) {
        qDebug() << "Socket not null";
        return;
    }

    if(ipaddr == "" || portnum == "") {
        qDebug() << "No IP address or port number";
        return;
    }

    //TCP Socket
    if ((TCPSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        qDebug() << "Cannot create TCP socket";
        return;
    }

    // Initialize and set up the address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(portnum.toStdString().c_str()));
    if ((hp = gethostbyname(ipaddr.toStdString().c_str())) == NULL)
    {
        formatMessage("Unknown server address");
        return;
    }
    // Copy the server address
    memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

    // Connecting to the server
    if (::connect(TCPSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        formatMessage("Can't connect to server");
        return;
    }

    //Get and send name
    if(username != "") {
        sendDataTCP(TCPSocket, username.toStdString().c_str());
        qDebug() << username;
    }
    else {
        qDebug() << "No name";
    }
    emit updateStatusBar(true);
    emit signalHandleRequest();
    emit finished();
}
void ThreadManager::voteForSong(char * song){
    sendDataTCP(TCPSocket, song);
}

void ThreadManager::VoiceConnect(QString clientIP)
{
    //TCP Socket
    if ((VCConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        qDebug() << "Cannot create TCP socket";
        return;
    }
    // Initialize and set up the address structure
    memset((char *)&server, 0, sizeof(struct sockaddr_in));
    VCserver.sin_family = AF_INET;
    VCserver.sin_port = htons(DEFAULT_VOICE_PORT);
    //This must be fixed
    if ((hp = gethostbyname(clientIP.toLocal8Bit().constData())) == NULL)
    {
        formatMessage("Unknown server address");
        return;
    }
    // Copy the server address
    memcpy((char *)&VCserver.sin_addr, hp->h_addr, hp->h_length);

    // Connecting to the server
    if (::connect(VCConnectSocket, (struct sockaddr*)&VCserver, sizeof(VCserver)) == SOCKET_ERROR)
    {
        formatMessage("Can't connect to client");
        return;
    }

    //send the username to the other client
    sendDataTCP(VCConnectSocket, mUsername.toStdString().c_str());

    udp.initalizeVoiceChatSockets(clientIP);
    qDebug() << "HOW DOES THIS GET HERE";
}
void ThreadManager::setupVoiceChat()
{
    int BytesRead;
    char buf[PACKET_LEN];
    int bytesToRead = PACKET_LEN;
    char *bp = buf;

    qDebug() << "Setup Voice Chat Thread Created";

    if(VCSocket != 0 && AcceptSocket != 0) {
        qDebug() << "Socket not null";
        closesocket(AcceptSocket);
        closesocket(VCSocket);
        printf("VC Socket: %d Accept Socket: %d", VCSocket, AcceptSocket);
    }
    if ((AcceptSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        qDebug() << "WSASocket: Failed to get an accept socket";
        return;
    }
    //Init address structs
    voiceChatClient.sin_family = AF_INET;
    voiceChatClient.sin_addr.s_addr = htonl(INADDR_ANY);
    voiceChatClient.sin_port = htons(DEFAULT_VOICE_PORT);

    if (bind(AcceptSocket, (PSOCKADDR)&voiceChatClient, sizeof(voiceChatClient)) == SOCKET_ERROR)
    {
        qDebug() << "bind() failed on server";
        return;
    }
    if (listen(AcceptSocket, 1))
    {
       qDebug() << "listen() failed on tcp server";
       return;
    }
    int client_len = sizeof(voiceChatClient);
    if((VCSocket = accept(AcceptSocket, (struct sockaddr *)&voiceChatClient, &client_len)) == INVALID_SOCKET)
    {
        closesocket(AcceptSocket);
        closesocket(VCSocket);
        qDebug() <<  "Can't accept UDP client";
        return;
    }
    else
    {
        qDebug() <<  "we accept UDP client";
    }
    char buf2[PACKET_LEN];
    char *voiceChatUsername = buf2;
    // handle the username that is sent
    if(receiveTCP(VCSocket, voiceChatUsername)){
        //add client to gui
        emit signalUpdateVoiceChatUser(voiceChatUsername);
        emit signalToggleVoiceButtons(true);
    } else
    {
        qDebug() << "TCP CANNOT RECEIVE IN SETUP VOICE CHAT";
    }
    connectionRequested = true;
    QString temp = inet_ntoa(voiceChatClient.sin_addr);
    udp.initalizeVoiceChatSockets(temp);
    emit signalStartPlayer();
}
bool ThreadManager::handleRequest()
{
    int BytesRead;
    char buf[PACKET_LEN];
    int bytesToRead = PACKET_LEN;
    char *bp = buf;
    qDebug() << "BEFORE RECV";
    /*if ((BytesRead = recv(TCPSocket, bp, bytesToRead, 0)) > 0)
    {
        bytesToRead -= BytesRead;
    }
    */
     BytesRead = WSARead(TCPSocket, bp, 5000, 1024);
    /* recv() failed */
    if(BytesRead < 0)
    {
        qDebug() << "recv() failed";
        return false;
    }
    /* client disconnected */
    if(BytesRead == 0)
    {
        qDebug() << "CLIENT DISCONNECTED";
        return false;
    }

    if (buf[0] == ERROR_BIT)
    {
        qDebug() << "ERROR BIT DETECTED";
        return false;
        //parseUserList(bp);
    }

    //if its a song/user refresh request, parse the list
    if (buf[0] == REFRESH_SONG || buf[0] == REFRESH_USER){
        parseUserList(bp);
    }
    qDebug() << "Received : "  << buf;
    return true;
}
void ThreadManager::disconnect()
{
    emit updateStatusBar(false);
    this->thread()->quit();
}
void ThreadManager::SendDownloadRequest(QString songName)
{
    HANDLE writeThread;
    DWORD id;
    char fileName[500] = { 0 };
    c.init();
    int left = 42344526;
    sendDataTCP(TCPSocket, songName.toStdString().c_str());
    songName = songName.remove(0, 1);
    char * title =  fileName;
    strcpy(title,songName.toStdString().c_str());


    //receiveTCP(TCPSocket, dontcare);
    if(!handleRequest())
        return;
    char buf[20000] = { 0 };
    int len;
    writeThread = CreateThread(NULL, 0, readStuff, (void *)title, 0 , &id);
    while((len = WSARead(TCPSocket, buf, 2000, 20000))){
        left -= len;
        c.push(buf, len);
        if(left == 0){
            printf("No data left");
            break;
        }
        //SetEvent(newData);
    }
    SetEvent(readDone);
    WaitForSingleObject(fileDone, 20000);
    emit signalDownloadStatus(2);
    printf("Done reading");
    fflush(stdout);
}
void ThreadManager::SendUploadRequest(QString songName)
{

    FILE * upload;
    char buffer[FILEMAX] = { 0 };
    QString songNameWithPath;

//    std::string temp;
//    temp = REQ_UPLOAD;
//    temp += "download.jpg";

    int index = 0;
    //find the corresponding song title with filepath
    for(QStringList::iterator it = uploadList.begin(); it != uploadList.end(); ++it, index++){
        QString current = *it;
        if(QString::compare(current,songName,Qt::CaseSensitive) == 0){
            songNameWithPath = uploadListWithPath.at(index);
            break;
        }
    }

    //prepend the upload request to the path
    songName = REQ_UPLOAD + songName;

    sendDataTCP(TCPSocket, songName.toStdString().c_str());
    char * buff = buffer;
    int bytesRead;
    if(!(upload = fopen(songNameWithPath.toStdString().c_str(), "rb+"))){

            qDebug() << songNameWithPath;
            qDebug() << "couldn't open file";
            return;
    }
    while((bytesRead = fread(buff, sizeof(char), FILEMAX, upload))){
        if(bytesRead != FILEMAX){
            WSAS(TCPSocket, buff, bytesRead, 1000);
            break;
        }
        WSAS(TCPSocket, buff, 20000, 1000);
    }
    qDebug() << "end song upload request function";
    emit signalUploadStatus(2);
}

void ThreadManager::SendVoiceRequest()
{
    std::string temp;
    temp = REQ_CHAT;
    sendDataTCP(TCPSocket, temp.c_str());
}
void ThreadManager::SendSongRefreshRequest()
{
    qDebug() << "Send Song Refresh Request is called";
    std::string temp;
    temp = REFRESH_SONG;
    sendDataTCP(TCPSocket, temp.c_str());
    if(!handleRequest())
        return;
}
void ThreadManager::SendVoiceRefreshRequest()
{

    qDebug() << "Send Voice Refresh Request is called";
    std::string temp;
    temp = REFRESH_USER;
    sendDataTCP(TCPSocket, temp.c_str());
    if(!handleRequest())
        return;
}

DWORD WINAPI uploadStuff(LPVOID param){
    char * title = (char *)param;
    FILE * upload;
    char buffer[FILEMAX] = { 0 };
    char * buff = buffer;
    int bytesRead;
    if(!(upload = fopen(title, "rb+"))){
            return -1;
    }
    while((bytesRead = fread(buff, sizeof(char), FILEMAX, upload))){
            if(bytesRead != FILEMAX){
                WSAS(TCPSocket, buff, bytesRead, 1000);
                break;
            }
            WSAS(TCPSocket, buff, 20000, 1000);
    }
}

DWORD WINAPI readStuff(LPVOID param){
    char * title = (char *)param;
    FILE * fqt;
    DWORD err;
    int len;
    char readBuff[20000] = { 0 };
    HANDLE events[2];
    events[0] = newData;
    events[1] = readDone;
    char * wrt;
    fqt = fopen(title, "wb");
    wrt = readBuff;
    while(1){
        err = WaitForMultipleObjects(2, events, FALSE, INFINITE);
        ResetEvent(newData);
        while(len = c.pop(wrt)){
            fwrite(wrt, sizeof(char), len, fqt);
            printf("head %d tail %d, length: %d\n", c.head, c.tail, len);
            memset(readBuff, '\0', 20000);
        }
        if(err == WAIT_OBJECT_0 + 1){
            SetEvent(fileDone);
            fclose(fqt);
            break;
        }

    }
    return 0;
}

void ThreadManager::parseUserList(char* bp)
{
    //Refresh client
    bool checkFlag = 0;
    QVector<QString> userList;
    std::string s(bp);
    size_t pos = 0;
    std::string userDelimiter = "&";
    std::string songDelimiter = "*";
    if((s.find(userDelimiter) != std::string::npos))
    {
        checkFlag = 0;
        pos = s.find(userDelimiter);
        s.erase(pos, userDelimiter.length());
    } else if ((s.find(songDelimiter) != std::string::npos))
    {
        checkFlag = 1;
        pos = s.find(songDelimiter);
        s.erase(pos, songDelimiter.length());
    }
    std::stringstream ss(s);
    std::string token;
    while(std::getline(ss, token, ';'))
    {
        QString tokenString = QString::fromUtf8(token.c_str());
        userList.push_back(tokenString);
    }
    if (checkFlag == 0){
        emit updateUserList(userList);
    }
    else if (checkFlag == 1){
        playlist = userList;
        emit updateSongList(userList);
    }
}

void ThreadManager::requestVoiceChatIP(QString username){
    std::string temp;
    temp = username.toUtf8().constData();
    sendDataTCP(TCPSocket, temp.c_str());
}
