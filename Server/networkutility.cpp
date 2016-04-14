#include "networkutility.h"

int sentBytes = 0;

SOCKADDR_IN  stLclAddr;
SOCKADDR_IN  client;
SOCKET StreamSocket;

networkutility::networkutility()
{

}

void networkutility::debugMessage(const char* message){
    qDebug() << message;
}

void sendDataTCP(SOCKET sd, const char* message){
    int sent;
    if((sent = send(sd, message, PACKET_LEN, 0)) == SOCKET_ERROR){
        return;
    }
    networkutility::debugMessage("Sending data to client. Sending:");
    networkutility::debugMessage(message);
}

bool networkutility::WSAS(SOCKET sd, char * message, int size, int timeout){
    WSABUF buf;
    WSAOVERLAPPED ov;
    ov.hEvent = WSACreateEvent();
    buf.buf = message;
    buf.len = size;
    DWORD sendBytes;
    DWORD sendErr;
    sendErr = WSASend(sd, &buf, 1, &sendBytes, 0, &ov,0);
    if((sendErr == SOCKET_ERROR) && (WSA_IO_PENDING != WSAGetLastError())) {
        sendErr = WSAGetLastError();
        WSACleanup();
        exit(0);
    }

    fflush(stdout);
    sendErr = WSAGetLastError();

    if (sendErr == WSA_IO_PENDING) {

        sendErr = WSAWaitForMultipleEvents(1, &ov.hEvent, FALSE, 1000, FALSE);
        if (sendErr == WSA_WAIT_FAILED) {
            printf("WSAWaitForMultipleEvents failed with error: %d\n",
                WSAGetLastError());
            exit(0);
        }
    }
    //int rec = WSAGetOverlappedResult(sd, &ov, &sendBytes, FALSE, (LPDWORD)0);
    sentBytes += sendBytes;
    printf("Sent BYTES: %d", sentBytes);
    return true;
}

bool receiveTCP(SOCKET sd, char* message){
    printf("RANDOMLY READING:::");
    fflush(stdout);
    if(recv(sd, message, PACKET_LEN, 0) == SOCKET_ERROR){
        return FALSE;
    }
    return TRUE;
}
int WSARead(SOCKET sd, char * message, int timeout, int size){
    WSAOVERLAPPED ov;
    DWORD recvBytes;
    DWORD recvErr;
    DWORD flags = 0;
    WSABUF dbuf;
    dbuf.buf = message;
    dbuf.len = size;
    ov.hEvent = WSACreateEvent();
    if ((recvErr = WSARecv(sd, &dbuf, 1, &recvBytes, &flags, &ov, NULL)) == SOCKET_ERROR)
    {
        recvErr = WSAGetLastError();
        if (recvErr != WSA_IO_PENDING)
        {

            return 0;
        }
    }

    recvErr = WSAWaitForMultipleEvents(1, &ov.hEvent, FALSE, timeout, FALSE);
    switch (recvErr) {
    case WAIT_TIMEOUT:
        return 0;
        break;
    case WAIT_FAILED:
        exit(1);
        break;
    default:
        break;
    }
    //int x = strlen(dbuf.buf);
    int rc = WSAGetOverlappedResult(sd, &ov, &recvBytes, FALSE, &flags);
    return recvBytes;
}
