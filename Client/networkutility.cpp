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
void formatMessage(const char* message)
{
    qDebug() << message;
}
