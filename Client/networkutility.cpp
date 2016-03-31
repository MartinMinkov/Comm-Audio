#include "networkutility.h"

void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, const char* buffer)
{
    /* zero out overlapped structure	*/
    ZeroMemory((&SOCKET_INFO->Overlapped), sizeof(WSAOVERLAPPED));
    SOCKET_INFO->BytesRECV = 0;
    SOCKET_INFO->BytesSEND = 0;
    SOCKET_INFO->DataBuf.len = PACKET_LEN;
    strcpy(SOCKET_INFO->DataBuf.buf, buffer);
}

void sendDataTCP(SOCKET sd, const char* message)
{
    if(send(sd, message, PACKET_LEN, 0) == SOCKET_ERROR)
    {
       if (WSAGetLastError())
        {
            formatMessage("Sending TCP data information failed");
            return;
        }
    }
    formatMessage("Sending Data to Server");
}
void sendDatalUDP(LPSOCKET_INFORMATION SI, struct	sockaddr_in server, char* message)
{
    initSockInfo(SI, message);

    //Send control data
    if (WSASendTo(SI->Socket, &(SI->DataBuf), 1, &SI->BytesSEND, 0, (struct sockaddr *)&server, sizeof(server), &(SI->Overlapped), NULL) == SOCKET_ERROR)
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
    if(recv(sd, message, PACKET_LEN, 0) == SOCKET_ERROR)
    {
        if (WSAGetLastError())
        {
            return FALSE;
        }
    }
    return TRUE;
}
bool WSARead(SOCKET sd, char * message, int timeout, int size){
    WSAOVERLAPPED ov;
    DWORD recvBytes;
    DWORD recvErr;
    DWORD flags = 0;
    WSABUF dbuf;
    dbuf.buf = message;
    dbuf.len = size;
    ov.hEvent = WSACreateEvent();
    if ((recvErr = WSARecv(sd, &dbuf, 1, &recvBytes,
                &flags, &ov, NULL)) == SOCKET_ERROR)
            {
                recvErr = WSAGetLastError();
                if (recvErr != WSA_IO_PENDING) {

                    return false;
                }
            }


            recvErr = WSAWaitForMultipleEvents(1, &ov.hEvent, FALSE, timeout, FALSE);
            switch (recvErr) {
            case WAIT_TIMEOUT:

                wchar_t buf[256];

                return false;
                break;
            case WAIT_FAILED:

                exit(1);
                // call GetLastError( )
                break;
            default:
                break;
            }
            printf("Done reading in WSARead: %s", message);
            fflush(stdout);
            return true;


}

void formatMessage(const char* message)
{
    qDebug() << message;
}
