#include "networkutility.h"


networkutility::networkutility()
{

}

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
        /*if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            formatMessage("Sending TCP data information failed");
            //ExitThread(1);
        }*/
    }
    formatMessage("Sending Data to Server");
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
