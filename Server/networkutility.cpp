#include "networkutility.h"

networkutility::networkutility()
{

}

void networkutility::debugMessage(const char* message){
    qDebug() << message;
}

void networkutility::sendDataTCP(SOCKET sd, const char* message){
    if(send(sd, message, PACKET_LEN, 0) == SOCKET_ERROR){
        return;
    }
    debugMessage("Sending data to client");
}

bool networkutility::receiveTCP(SOCKET sd, char* message){
    if(recv(sd, message, PACKET_LEN, 0) == SOCKET_ERROR){
        return FALSE;
    }
    return TRUE;
}
