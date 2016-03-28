#include "clienthandlerthread.h"
#include "server.h"

ClientHandlerThread::ClientHandlerThread(int socket)
{
    m_socket = socket;
}

void ClientHandlerThread::receiveRequests(){
    qDebug("inside receiveRequests");

    int bytesRead;
    char * buf;
    buf = (char*)malloc(PACKET_LEN);
    memset(buf, 0, PACKET_LEN);
    char * bp = buf;
    int recvStatus;

    while(1){
        if((recvStatus = recv(m_socket, bp, PACKET_LEN, 0)) == SOCKET_ERROR){
            server::debugMessage("failed recv");
            return;
        }

        if(recvStatus == 0){
            server::debugMessage("client disconnected");
            break;
        }

        server::debugMessage(bp);
    }




//    if((recvStatus = recv(m_socket, bp, PACKET_LEN, 0)) == SOCKET_ERROR){
//        server::debugMessage("failed recv");
//        return;
//    }

//    server::debugMessage(bp);


//    while(1){
//        if((recvStatus = recv(m_socket, bp, PACKET_LEN, 0)) == SOCKET_ERROR){
//            server::debugMessage("failed recv");
//            return;
//        }

//        if(recvStatus == 0)
//            server::debugMessage("client disconnected");
//            break;

//        server::debugMessage(bp);
//    }



//    while(1){
//        int bytesToRead = PACKET_LEN;
//        char * bp = buf;
//        while((bytesRead = recv(m_socket, bp, bytesToRead, 0)) < PACKET_LEN){
//            if(bytesRead == 0)
//                break;

//            bytesToRead -= bytesRead;
//            bp += bytesRead;
//        }

//        // recv failed
//        if(bytesRead < 0){
//            server::debugMessage("recv() failed");
//            return;
//        }

//        // client disconnected
//        if(bytesRead == 0){
//            server::debugMessage("client disconnected");
//            return;
//        }

//        // handle data received
//        server::debugMessage(buf);
//    }
//    free(buf);

    emit finished();
}
