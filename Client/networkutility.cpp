#include "networkutility.h"
#include "mybuffer.h"

#include <QDebug>

SOCKET TCPSocket;
SOCKET AcceptSocket;
SOCKET StreamSocket;

SOCKET VCSocket;
SOCKET VCRecieveSocket;
SOCKET VCSendSocket;
SOCKET VCConnectSocket;

bool connectionRequested = false;
struct sockaddr_in streamServer;
LPSOCKET_INFORMATION SI;
QObject * mw;

void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, char* buffer, struct sockaddr_in server)
{
    /* zero out overlapped structure	*/
    ZeroMemory((&SOCKET_INFO->Overlapped), sizeof(WSAOVERLAPPED));
    SOCKET_INFO->BytesRECV = 0;
    SOCKET_INFO->BytesSEND = 0;
    SOCKET_INFO->DataBuf.len = BUFFSIZE;
    SOCKET_INFO->DataBuf.buf = buffer;
    SOCKET_INFO->server = server;
}

void sendDataTCP(SOCKET sd, const char* message)
{
    int check;
    if((check = send(sd, message, PACKET_LEN, 0)) == SOCKET_ERROR)
    {
       if (WSAGetLastError())
        {
            formatMessage("Sending TCP data information failed");
            return;
        }
    }
    formatMessage("Sending Data to Server");
}
void sendDatalUDP(LPSOCKET_INFORMATION SI, struct sockaddr_in server, char* message)
{
    initSockInfo(SI, message, server);

    //Send control data
    if (WSASendTo(SI->Socket, &(SI->DataBuf), 1, &SI->BytesSEND, 0, (struct sockaddr *)&streamServer, sizeof(streamServer), &(SI->Overlapped), NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            formatMessage("Sending control information failed");
            ExitThread(1);
        }
    }
    formatMessage("Sending Control Data to Server");
}
bool receiveTCP(SOCKET sd, char* message)
{
    qDebug() << "CALLING RECEIVE TCP DONT EAT MY PAACKETS";
    if(recv(sd, message, PACKET_LEN, 0) == SOCKET_ERROR)
    {
        if (WSAGetLastError())
        {
            return FALSE;
        }
    }
    return TRUE;
}
bool WSAS(SOCKET sd, char * message, int size, int timeout){
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
    if ((recvErr = WSARecv(sd, &dbuf, 1, &recvBytes,&flags, &ov, NULL)) == SOCKET_ERROR)
    {
        recvErr = WSAGetLastError();
        if (recvErr != WSA_IO_PENDING) {

            return 0;
        }
    }

    recvErr = WSAWaitForMultipleEvents(1, &ov.hEvent, FALSE, timeout, FALSE);
    switch (recvErr)
    {
    case WAIT_TIMEOUT:
        return 0;
        break;
    case WAIT_FAILED:
        exit(1);
        break;
    default:
        break;
    }
    int x = strlen(dbuf.buf);
    int rc = WSAGetOverlappedResult(sd, &ov, &recvBytes, FALSE, &flags);
    return recvBytes;

}
int receiveUDP(LPSOCKET_INFORMATION SI, sockaddr_in server, DWORD RecvBytes, DWORD Flags)
{
    int server_len;
    server_len = sizeof(streamServer);
    if (WSARecvFrom(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, (struct sockaddr *)&streamServer, &server_len, &(SI->Overlapped), ServerRoutine) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            //int i = WSAGetLastError();
            return FALSE;
        }
    }
    return TRUE;
}
void CALLBACK ServerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
    DWORD RecvBytes = 0, Index;
    DWORD Flags = 0;
    WSAEVENT			EventArray[1] = { 0 };
    WSAEVENT			acceptEvent;

    // Reference the WSAOVERLAPPED structure as a SOCKET_INFORMATION structure
    LPSOCKET_INFORMATION SOCKINFO = (LPSOCKET_INFORMATION)Overlapped;
    initSockInfo(SOCKINFO, SOCKINFO->Buffer, SOCKINFO->server);

    if (Error != 0)
    {
        qDebug() << "I/O operation failed with value" + Error;
        printf("%d", Error);
        fflush(stdout);
        GlobalFree(SOCKINFO);
        SetEvent(streamStop);
        return;
    }
    if ((acceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
    {
        qDebug() <<"WSACreateEvent() failed";
        GlobalFree(SOCKINFO);
        return;
    }

    Index = WSAWaitForMultipleEvents(1, EventArray, FALSE, 10000000, TRUE);

    if (Index == WSA_WAIT_TIMEOUT)
    {
        qDebug() <<"Timeout in UDP Server";
        GlobalFree(SOCKINFO);
        return;
    }
    receiveUDP(SOCKINFO, streamServer, BytesTransferred, Flags);
    cData.push(SOCKINFO->Buffer, 60000);
}
void formatMessage(const char* message)
{
    qDebug() << message;
}
