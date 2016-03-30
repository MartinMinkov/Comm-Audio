#include "networkutility.h"

networkutility::networkutility()
{

}

void networkutility::debugMessage(const char* message){
    qDebug() << message;
}

void sendDataTCP(SOCKET sd, const char* message){
    if(send(sd, message, PACKET_LEN, 0) == SOCKET_ERROR){
        return;
    }
    networkutility::debugMessage("Sending data to client");
}

bool receiveTCP(SOCKET sd, char* message){
    if(recv(sd, message, PACKET_LEN, 0) == SOCKET_ERROR){
        return FALSE;
    }
    return TRUE;
}
