#include "clienthandlerthread.h"
#include "server.h"
#include "networkutility.h"
#include "filehelper.h"
#include "streamhelper.h"
#include "chathelper.h"

ClientHandlerThread::ClientHandlerThread(int socket)
{
    m_socket = socket;
    fHelper = new filehelper();
    sHelper = new streamhelper();
    cHelper = new chathelper();
}

void ClientHandlerThread::receiveRequests(){
    qDebug("inside receiveRequests");

    int bytesRead;
    char buf[PACKET_LEN];

    while(1){

        int bytesToRead = PACKET_LEN;
        char *bp = buf;

        if((bytesRead = recv(m_socket, bp, bytesToRead, 0)) > 0){
            bytesToRead -= bytesRead;
        }

        if(bytesRead < 0){
            networkutility::debugMessage("failed recv");
            emit signalDisconnect();
            return;
        }

        if(bytesRead == 0){
            networkutility::debugMessage("client disconnected");
            emit signalDisconnect();
            break;
        }

        // download request
        if(buf[0] == REQ_DOWNLOAD){
            fHelper->handleDownloadRequest();
        }

        // upload request
        if(buf[0] == REQ_UPLOAD){
            fHelper->handleUploadRequest();
        }

        // stream request
        if(buf[0] == REQ_STREAM){
            sHelper->handleStreamRequest();
        }

        // chat request
        if(buf[0] == REQ_CHAT){
            cHelper->handleChatRequest();
        }


        networkutility::debugMessage(bp);
    }

    emit finished();
}

void ClientHandlerThread::disconnect(){
    networkutility::debugMessage("disconnecting client handler...");
    closesocket(m_socket);
    WSACleanup();
    emit finished();
}
