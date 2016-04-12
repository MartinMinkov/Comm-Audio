#include "chathelper.h"

chathelper::chathelper()
{

}

void chathelper::handleChatRequest(){
    networkutility::debugMessage("chat requested");
}

void chathelper::handleChatIPRequest(int socket, QString username){
    //find a matching username in the clientIP table, send it back to client
    if(userListWithIP.contains(username)){
        sendDataTCP(socket, userListWithIP.value(username).toLocal8Bit().data());
    } else {
        qDebug() << "no client with this username found";
    }
}
