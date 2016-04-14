#include "tcpthreadmanager.h"
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: tcpthreadmanager.cpp
--
-- FUNCTIONS:
-- ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
-- ThreadManager::~ThreadManager(){}
-- void ThreadManager::connect(QString ipaddr, QString portnum, QString username)
-- void ThreadManager::voteForSong(char * song)
-- void ThreadManager::VoiceConnect(QString clientIP)
-- void ThreadManager::setupVoiceChat()
-- bool ThreadManager::handleRequest()
-- void ThreadManager::disconnect()
-- void ThreadManager::SendDownloadRequest(QString songName)
-- void ThreadManager::SendUploadRequest(QString songName)
-- void ThreadManager::SendVoiceRequest()
-- void ThreadManager::SendSongRefreshRequest()
-- void ThreadManager::SendVoiceRefreshRequest()
-- DWORD WINAPI uploadStuff(LPVOID param)
-- DWORD WINAPI readStuff(LPVOID param)
-- void ThreadManager::parseUserList(char* bp)
-- void ThreadManager::requestVoiceChatIP(QString username)
--
-- push(char * buf)
-- clear()
--
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose & Martin Minkov
-- PROGRAMMER:  Colin Bose & Martin Minkov
--
-- NOTES: This class is responsible for supproting sending all data via the TCP control channel. It sends messages
            to the server as well as reading from the control channel as well.
----------------------------------------------------------------------------------------------------------------------*/
HANDLE newData;
HANDLE readDone;
HANDLE fileDone;
circlebuff c;
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ThreadManager
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose & Martin Minkov
-- PROGRAMMER:  Colin Bose & Martin Minkov
-- INTERFACE:	ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
--
--
-- NOTES: Constructor for the ThreadManager class.
----------------------------------------------------------------------------------------------------------------------*/
ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{
    TCPSocket = 0;
    VCSocket = 0;
    StreamSocket = 0;
    newData = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)L"1");
    readDone = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)L"2");
    fileDone = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)L"3");
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ThreadManager
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose & Martin Minkov
-- PROGRAMMER:  Colin Bose & Martin Minkov
-- INTERFACE:	ThreadManager::~ThreadManager(){}
--
--
-- NOTES: De-constructor for the ThreadManager class.
----------------------------------------------------------------------------------------------------------------------*/
ThreadManager::~ThreadManager(){}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getTime
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov & Allen Tsang
-- PROGRAMMER:  Martin Minkov & Allen Tsang
-- INTERFACE:	void ThreadManager::connect(QString ipaddr, QString portnum, QString username)
--
--
-- RETURNS: VOID
-- NOTES: Attempts to connect to the ip and port that is passed in. If successfull, it updates the GUI and listens for
            the number of users currently connected to the server.
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::connect(QString ipaddr, QString portnum, QString username)
{
    mUsername = username;

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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: voteForSong
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void ThreadManager::voteForSong(char * song
--
--
-- RETURNS: VOID
-- NOTES: Sends a message to the server via TCP to indicate a vote for the currently playing song
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::voteForSong(char * song){
    sendDataTCP(TCPSocket, song);
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: VoiceConnect
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	void ThreadManager::VoiceConnect(QString clientIP)
--
--
-- RETURNS: VOID
-- NOTES: Attempts to connect to the listening client for voice chat. If successfull, it initializes the UDP sockets
            so communication is ready.
----------------------------------------------------------------------------------------------------------------------*/
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
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setupVoiceChat
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	void ThreadManager::setupVoiceChat()
--
--
-- RETURNS: VOID
-- NOTES: Sets up a listening thread for voice chat.
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::setupVoiceChat()
{
    char buf[PACKET_LEN];

    if(VCSocket != 0 && AcceptSocket != 0) {
        qDebug() << "Socket not null";
        closesocket(AcceptSocket);
        closesocket(VCSocket);
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

    char buf2[PACKET_LEN];
    char *voiceChatUsername = buf2;

    // handle the username that is sent
    if(receiveTCP(VCSocket, voiceChatUsername)){
        //add client to gui
        emit signalUpdateVoiceChatUser(voiceChatUsername);
        emit signalToggleVoiceButtons(true);
    }

    connectionRequested = true;
    QString temp = inet_ntoa(voiceChatClient.sin_addr);
    udp.initalizeVoiceChatSockets(temp);
    emit signalStartPlayer();
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: handleRequest
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov & Colin Bose
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	bool ThreadManager::handleRequest()
--
--
-- RETURNS: BOOL - if the function received valid data or not.
-- NOTES: Attempts to read data on the TCP port.
----------------------------------------------------------------------------------------------------------------------*/
bool ThreadManager::handleRequest()
{
    int BytesRead;
    char buf[PACKET_LEN];
    char *bp = buf;

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
    }

    //if its a song/user refresh request, parse the list
    if (buf[0] == REFRESH_SONG || buf[0] == REFRESH_USER){
        parseUserList(bp);
    }
    return true;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: disconnect
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov & Alvin Man
-- PROGRAMMER:  Martin Minkov & Alvin Man
-- INTERFACE:	void ThreadManager::disconnect()
--
--
-- RETURNS: VOID
-- NOTES: Updates the main GUI and kills the calling thread.
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::disconnect()
{
    emit updateStatusBar(false);
    this->thread()->quit();
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SendDownloadRequest
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void ThreadManager::SendDownloadRequest(QString songName)
--
--
-- RETURNS: VOID
-- NOTES:
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::SendDownloadRequest(QString songName)
{
    HANDLE writeThread;
    DWORD id;
    char fileName[500] = { 0 };
    c.init();
    int left = 0;
    char fileSize[1024] = { 0 };
    char * fsz = fileSize;
    sendDataTCP(TCPSocket, songName.toStdString().c_str());
    songName = songName.remove(0, 1);
    char * title =  fileName;
    strcpy(title,songName.toStdString().c_str());


    receiveTCP(TCPSocket, fsz);
    if(fsz[0] == ERROR_BIT)
        return;
    printf(fsz);
    left = atoi(fsz);
    char buf[20000] = { 0 };
    int len;
    writeThread = CreateThread(NULL, 0, readStuff, (void *)title, 0 , &id);
    while((len = WSARead(TCPSocket, buf, 2000, 20000))){
        left -= len;
        c.push(buf, len);
        SetEvent(newData);
        if(left <= 0){
           break;
        }

    }
    SetEvent(readDone);
    WaitForSingleObject(fileDone, 20000);
    ResetEvent(fileDone);
    emit signalDownloadStatus(2);
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SendUploadRequest
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void ThreadManager::SendUploadRequest(QString songName)
--
--
-- RETURNS: VOID
-- NOTES:
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::SendUploadRequest(QString songName)
{
    FILE * upload;
    char buffer[FILEMAX] = { 0 };
    QString songNameWithPath;

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
    emit signalUploadStatus(2);
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SendVoiceRequest
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov & Allen Tsang
-- PROGRAMMER:  Martin Minkov & Allen Tsang
-- INTERFACE:	void ThreadManager::SendVoiceRequest()
--
--
-- RETURNS: VOID
-- NOTES:   Attempts to send a control message to the server indicating to refresh the user list on voice chat
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::SendVoiceRequest()
{
    std::string temp;
    temp = REQ_CHAT;
    sendDataTCP(TCPSocket, temp.c_str());
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SendSongRefreshRequest
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov & Allen Tsang
-- PROGRAMMER:  Martin Minkov & Allen Tsang
-- INTERFACE:	void ThreadManager::SendSongRefreshRequest()
--
--
-- RETURNS: VOID
-- NOTES:   Attempts to send a control message to the server indicating to refresh the song list
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::SendSongRefreshRequest()
{
    std::string temp;
    temp = REFRESH_SONG;
    sendDataTCP(TCPSocket, temp.c_str());
    if(!handleRequest())
        return;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: SendVoiceRefreshRequest
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov & Allen Tsang
-- PROGRAMMER:  Martin Minkov & Allen Tsang
-- void ThreadManager::SendVoiceRefreshRequest()
--
--
-- RETURNS: VOID
-- NOTES: Attempts to send a control message to the server.
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::SendVoiceRefreshRequest()
{
    std::string temp;
    temp = REFRESH_USER;
    sendDataTCP(TCPSocket, temp.c_str());
    if(!handleRequest())
        return;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: uploadStuff
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	DWORD WINAPI uploadStuff(LPVOID param)
--
--
-- RETURNS: DWORD
-- NOTES:
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: readStuff
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	DWORD WINAPI readStuff(LPVOID param)
--
--
-- RETURNS: DWORD
-- NOTES:
----------------------------------------------------------------------------------------------------------------------*/
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
            memset(readBuff, '\0', 20000);
        }
        if(err == WAIT_OBJECT_0 + 1){
            SetEvent(fileDone);
            fclose(fqt);
            c.clear();
            ResetEvent(readDone);
            break;
        }

    }
    return 0;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: parseUserList
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov & Alvin Man
-- PROGRAMMER:  in Minkov & Alvin Man
-- INTERFACE:	void ThreadManager::parseUserList(char* bp)
--
--
-- RETURNS: VOID
-- NOTES: Updates the GUI based on the message passed in.
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: requestVoiceChatIP
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE:	void ThreadManager::requestVoiceChatIP(QString username)
--
--
-- RETURNS: VOID
-- NOTES: Sends a request message to the server indicating to send back the IP address of the highlighted user
            so a TCP voice chat connect can be initiated.
----------------------------------------------------------------------------------------------------------------------*/
void ThreadManager::requestVoiceChatIP(QString username){
    std::string temp;
    temp = username.toUtf8().constData();
    sendDataTCP(TCPSocket, temp.c_str());
}
