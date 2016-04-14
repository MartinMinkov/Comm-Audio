#include "clienthandlerthread.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: clienthandlerthread.cpp
--
-- FUNCTIONS:
-- ClientHandlerThread::ClientHandlerThread(int socket, myBuffer * player, QString clientIP)
-- void ClientHandlerThread::receiveRequests()
-- void ClientHandlerThread::setupStream()
-- void ClientHandlerThread::startStream()
-- void ClientHandlerThread::disconnect()
-- QString ClientHandlerThread::constructUserListString()
-- std::string ClientHandlerThread::constructSongListString()
-- void ClientHandlerThread::removeUserFromList()
--
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
--
----------------------------------------------------------------------------------------------------------------------*/

myBuffer * playBuff;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ClientHandlerThread
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: ClientHandlerThread(int socket, myBuffer * player, QString clientIP)
--
-- NOTES: Constructor for client handler thread.
----------------------------------------------------------------------------------------------------------------------*/
ClientHandlerThread::ClientHandlerThread(int socket, myBuffer * player, QString clientIP)
{
    playBuff = player;
    m_socket = socket;
    m_clientIP = clientIP;
    fHelper = new filehelper();
    sHelper = new streamhelper();
    cHelper = new chathelper();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: receiveRequests
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: receiveRequests()
--
-- RETURNS: VOID
-- NOTES: Handles incoming requests to the clients.
----------------------------------------------------------------------------------------------------------------------*/
void ClientHandlerThread::receiveRequests(){

//    qRegisterMetaType<QVector<QString>>("userVector");
    qDebug("inside receiveRequests");

    int bytesRead;
    char buf[PACKET_LEN];
    char *username = buf;
    char usernameBuffer[50] = { 0 };
    // handle the username that is sent
    if(receiveTCP(m_socket, username)){
        networkutility::debugMessage("username:");
        networkutility::debugMessage(username);
        clientUsername = usernameBuffer;
        strcpy(clientUsername, username);
        userList.push_back(username);
        qDebug() << "adding username";
        qDebug() << clientUsername;
        //add client username and ip to clientIP map
        userListWithIP.insert(clientUsername, m_clientIP);
//        QString test = userListWithIP.value(clientUsername);

        //add client to gui
        emit signalUpdateUserList(userList);
    }
    playBuff->setSocket(m_socket);
    // send the server the username list
    QString constructedUserList = constructUserListString();
    sendDataTCP(m_socket, constructedUserList.toLocal8Bit().data());

    while(1){

        int bytesToRead = PACKET_LEN;
        char *bp = buf;

        if((bytesRead = recv(m_socket, bp, bytesToRead, 0)) > 0){
            bytesToRead -= bytesRead;
            printf("Read something: %s", buf);
        }

        if(bytesRead < 0){
            networkutility::debugMessage("failed recv");
            printf("Error: %d", bytesRead);
            emit signalHandlerDisconnect();
            return;
        }

        if(bytesRead == 0){
            networkutility::debugMessage("client disconnected");
            emit signalHandlerDisconnect();
            break;
        }

        // download request
        if(buf[0] == REQ_DOWNLOAD){
            fHelper->handleDownloadRequest(buf, m_socket);
        }

        // upload request
        if(buf[0] == REQ_UPLOAD){
            fHelper->handleUploadRequest(buf, m_socket);
        }
        if(buf[0] == VOTE){
            bp = &buf[0];
            bp++;
            int vote = atoi(bp);
            songVoting[vote]++;
            printf("Voting for :%d", vote);
            fflush(stdout);

        }

        // stream request
        if(buf[0] == REQ_STREAM){
            sHelper->handleStreamRequest();
        }

        // chat request
        if(buf[0] == REQ_CHAT){
            cHelper->handleChatRequest();
        }

        // chat ip request
        if(buf[0] == REQ_CHAT_IP){
            std::string user_name(bp);
            user_name.erase(0, 1);
            QString temp = QString::fromStdString(user_name);
            cHelper->handleChatIPRequest(m_socket, temp);
        }

        if(buf[0] == REFRESH_SONG){
            qDebug() << "Send Refresh is caught";
            std::string constructedSongList = constructSongListString();
            networkutility::debugMessage(constructedSongList.c_str());
            sendDataTCP(m_socket, constructedSongList.c_str());
        }

        // user refresh request
        if(buf[0] == REFRESH_USER){
            QString refreshedUserList = constructUserListString();
            sendDataTCP(m_socket, refreshedUserList.toLocal8Bit().data());
        }
    }

    emit finished();
}

void ClientHandlerThread::setupStream(){
    playBuff = new myBuffer(m_socket);
}
void ClientHandlerThread::startStream(){
    playBuff->startPlayer();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: disconnect
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: disconnect()
--
-- RETURNS: VOID
-- NOTES: Logic to handle cleanup and disconnecting of a client.
----------------------------------------------------------------------------------------------------------------------*/
void ClientHandlerThread::disconnect(){
    networkutility::debugMessage("disconnecting client handler...");
//    QVector<std::string>::iterator it = std::find(userList.begin(), userList.end(), clientUsername);

    removeUserFromList();
    emit signalUpdateUserList(userList);
    closesocket(m_socket);
    //WSACleanup();
    emit finished();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: constructUserListString
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: constructUserListString()
--
-- RETURNS: VOID
-- NOTES: Constructs the user list to be sent to the client.
----------------------------------------------------------------------------------------------------------------------*/
QString ClientHandlerThread::constructUserListString(){
    QString userListString = "";
    userListString.append(REFRESH_USER);
        int i = 0;

        for (auto it = userList.cbegin(); it != userList.cend(); it++, i++)
        {
            userListString.append(*it).append(";");
        }
        userListString.remove('\n');
        return userListString;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: constructSongListString
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: constructSongListString()
--
-- RETURNS: VOID
-- NOTES: Constructs the song list to be sent to the client.
----------------------------------------------------------------------------------------------------------------------*/
std::string ClientHandlerThread::constructSongListString(){
    std::string songListString = "";
    songListString += REFRESH_SONG;

    QString temp;
    if(playlistModel){
        for(int i = 0; i < playlistModel->rowCount(); i++){
            temp = playlistModel->index(i, 1).data(Qt::DisplayRole).toString();
            songListString += temp.toUtf8().constData();
            songListString += ";";
        }
    }
    qDebug(songListString.c_str());

    return songListString;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: removeUserFromList
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: removeUserFromList()
--
-- RETURNS: VOID
-- NOTES: Removes the username from the list when a client disconnects
----------------------------------------------------------------------------------------------------------------------*/
void ClientHandlerThread::removeUserFromList(){
    //logic to find the username in the userlist and remove it if a client disconnects
    int i = 0;
    for (auto it = userList.cbegin(); it != userList.cend(); it++, i++)
    {
        if(*it == clientUsername){
            userList.remove(i);
            QString constructedUserList = constructUserListString();
            networkutility::debugMessage(constructedUserList.toLocal8Bit().data());
            break;
        }
    }

    //also remove it from the clientIP map
    userListWithIP.remove(clientUsername);
}

