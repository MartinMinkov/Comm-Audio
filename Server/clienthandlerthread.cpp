#include "clienthandlerthread.h"

myBuffer * playBuff;
ClientHandlerThread::ClientHandlerThread(int socket, myBuffer * player, QString clientIP)
{
    playBuff = player;
    m_socket = socket;
    m_clientIP = clientIP;
    fHelper = new filehelper();
    sHelper = new streamhelper();
    cHelper = new chathelper();
}

void ClientHandlerThread::receiveRequests(){

//    qRegisterMetaType<QVector<QString>>("userVector");
    qDebug("inside receiveRequests");

    int bytesRead;
    char buf[PACKET_LEN];
    char *username = buf;

    // handle the username that is sent
    if(receiveTCP(m_socket, username)){
        networkutility::debugMessage("username:");
        networkutility::debugMessage(username);
        clientUsername = username;
        userList.push_back(username);

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
//            continue;
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

void ClientHandlerThread::disconnect(){
    networkutility::debugMessage("disconnecting client handler...");
//    QVector<std::string>::iterator it = std::find(userList.begin(), userList.end(), clientUsername);

    removeUserFromList();
    emit signalUpdateUserList(userList);
    closesocket(m_socket);
    WSACleanup();
    emit finished();
}

QString ClientHandlerThread::constructUserListString(){
    QString userListString = "";
    userListString.append(REFRESH_USER);
        int i = 0;

        for (auto it = userList.cbegin(); it != userList.cend(); it++, i++)
        {
            userListString.append(*it).append(";");
        }

        //erase newline chars
//        userListString.erase(std::remove(userListString.begin(), userListString.end(), '\n'), userListString.end());

        userListString.remove('\n');

        return userListString;
}

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

