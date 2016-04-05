#include "udpthreadmanager.h"

UDPThreadManager::UDPThreadManager(QObject *parent) : QObject(parent)
{
    StreamSocket = 0;
}
void UDPThreadManager::initMultiCastSock()
{
    int nRet;
    bool fFlag;
    StreamSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (StreamSocket == INVALID_SOCKET)
    {
        qDebug() << "Cannot create UDP socket";
        return;
    }
    fFlag = true;
    nRet = setsockopt(StreamSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));
    if (nRet == SOCKET_ERROR)
    {
        qDebug() << "Set SockOpt Failed";
        return;
    }
    server.sin_family      = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port        = htons(DEFUALT_STREAM_PORT);
    int addr_size = sizeof(struct sockaddr_in);
    nRet = bind(StreamSocket, (struct sockaddr*) &server, addr_size);
    if (nRet == SOCKET_ERROR)
    {
        qDebug() << "bind( failed";
        return;
    }
    //Join Multicast Group
    stMreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS);
    stMreq.imr_interface.s_addr = INADDR_ANY;
    nRet = setsockopt(StreamSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
    if (nRet == SOCKET_ERROR)
    {
        qDebug() << "setsockopt() IP_ADD_MEMBERSHIP address failed";
        return;
    }

    //Setup UDP completion routine
}
void UDPThreadManager::UDPWorker()
{
    DWORD RecvBytes = 0, Index;
    DWORD Flags = 0;
    WSAEVENT				UDPEvent;
    WSAEVENT				EventArray[1];

    //Creating Socket Info struct
    if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
    {
        qDebug() << "GlobalAlloc() failed with error";
        return;
    }
    if ((UDPEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
    {
        qDebug() << "WSACreateEvent() failed";
        return;
    }

    // Save the event in the event array.
    EventArray[0] = UDPEvent;

    //Copy socket
    SI->Socket = StreamSocket;
    // Fill in the details of our socket.
    initSockInfo(SI, SI->Buffer);

    receiveUDP(SI, server, RecvBytes, Flags);
}
void UDPThreadManager::receiveStream()
{
    int nRet;
    char bp [PACKET_LEN];
    while (1)
    {
        int addr_size = sizeof(struct sockaddr_in);
        nRet = recvfrom(StreamSocket, bp, PACKET_LEN, 0, (struct sockaddr*)&server, &addr_size);
        if (nRet < 0)
        {
            qDebug() << "recvfrom() failed";
            return;
        }
        qDebug() << "Received in multi cast : " << bp;
    }
}
bool UDPThreadManager::receiveUDP(LPSOCKET_INFORMATION SI, sockaddr_in server, DWORD RecvBytes, DWORD Flags)
{
    int server_len;
    server_len = sizeof(server);
    if (WSARecvFrom(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, (struct sockaddr *)&server, &server_len, &(SI->Overlapped), ServerRoutine) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            int i = WSAGetLastError();
            return FALSE;
        }
    }
    return TRUE;
}
void CALLBACK UDPThreadManager::ServerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags)
{
    DWORD RecvBytes = 0, Index;
    DWORD Flags = 0;
    WSAEVENT			EventArray[1] = { 0 };
    WSAEVENT			acceptEvent;

    // Reference the WSAOVERLAPPED structure as a SOCKET_INFORMATION structure
    LPSOCKET_INFORMATION SI = (LPSOCKET_INFORMATION)Overlapped;
    initSockInfo(SI, SI->Buffer);

    if (Error != 0)
    {
        int i = WSAGetLastError();
        qDebug() << "I/O operation failed";
    }

    initSockInfo(SI, SI->Buffer);
    if ((acceptEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
    {
        qDebug() <<"WSACreateEvent() failed";
        return;
    }
    Index = WSAWaitForMultipleEvents(1, EventArray, FALSE, 2000, TRUE);

    if (Index == WSA_WAIT_TIMEOUT)
    {
        qDebug() <<"Timeout in UDP Server";
        return;
    }
    receiveUDP(SI, server, BytesTransferred, Flags);

}
