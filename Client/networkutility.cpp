#include "networkutility.h"
#include "mybuffer.h"
#include <QDebug>
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: networkutility.cpp
--
-- FUNCTIONS:
-- void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, char* buffer, struct sockaddr_in server)
-- void sendDataTCP(SOCKET sd, const char* message)
-- bool receiveTCP(SOCKET sd, char* message)
-- int receiveUDP(LPSOCKET_INFORMATION SI, sockaddr_in server, DWORD RecvBytes, DWORD Flags)
-- void CALLBACK ServerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
-- void formatMessage(const char* message)
--
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose & Martin Minkov
-- PROGRAMMER:  Colin Bose & Martin Minkov
--
-- NOTES: A utility class that holds wrapper functions for sending and receiving TCP and UDP data. Also contains the
            completion routine used for UDP.
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: initSockInfo
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, char* buffer, struct sockaddr_in server)
--
--
-- RETURNS: VOID
-- NOTES: Sets the LPSOCKET_INFORMATION struct information to 0.
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: sendDataTCP
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	void sendDataTCP(SOCKET sd, const char* message)
--
--
-- RETURNS: VOID
-- NOTES: Sends the passed in message through the TCP socket.
----------------------------------------------------------------------------------------------------------------------*/
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
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: receiveTCP
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	bool receiveTCP(SOCKET sd, char* message)
--
--
-- RETURNS: BOOL
            - True if the function is successfull, otherwise false if it fails
-- NOTES: Receives data on the TCP socket and stores it inside the passed in message parameter
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WSAS
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: bool networkutility::WSAS(SOCKET sd, char * message, int size, int timeout)
--                          SOCKET sd      - socket to send to
--                          char * message - message to write to the socket
--                          int size       - length of the message to send
--                          int timeout    - length of time to wait for timeout
-- RETURNS: BOOL - true = success, false = TIMEOUT
-- NOTES: Call to send an async TCP send.
----------------------------------------------------------------------------------------------------------------------*/

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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: WSARead
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: int WSARead(SOCKET sd, char * message, int timeout, int size)
--                      SOCKET sd   - socket to read
--                      char * message - buffer to copy the data received to
--                      int timeout  - time to wait for timeout
--                      int size     - amount of data to read
-- RETURNS: INT - number of bytes read
-- NOTES: Call to do WSA read of a socket
----------------------------------------------------------------------------------------------------------------------*/

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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: receiveUDP
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	int receiveUDP(LPSOCKET_INFORMATION SI, sockaddr_in server, DWORD RecvBytes, DWORD Flags)
--
--
-- RETURNS: int
            - 1 if the function is successfull, otherwise 0 if it fails
-- NOTES: Call to initialize the circular buffer.
----------------------------------------------------------------------------------------------------------------------*/
int receiveUDP(LPSOCKET_INFORMATION SI, sockaddr_in server, DWORD RecvBytes, DWORD Flags)
{
    int server_len;
    server_len = sizeof(streamServer);
    if (WSARecvFrom(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, (struct sockaddr *)&streamServer, &server_len, &(SI->Overlapped), ServerRoutine) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            return FALSE;
        }
    }
    return TRUE;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: ServerRoutine
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	void CALLBACK ServerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
--
--
-- RETURNS: VOID
-- NOTES: The completion routine used for UDP receiving. Constantly pushes valid data received into the circ buff
            so it can be used to play audio later.
----------------------------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: formatMessage
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Martin Minkov
-- PROGRAMMER:  Martin Minkov
-- INTERFACE:	void formatMessage(const char* message)
--
--
-- RETURNS: VOID
-- NOTES: Prints the passed in message through qDebug()
----------------------------------------------------------------------------------------------------------------------*/
void formatMessage(const char* message)
{
    qDebug() << message;
}
