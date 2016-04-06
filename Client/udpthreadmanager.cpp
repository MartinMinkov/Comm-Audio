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
    streamServer.sin_family      = AF_INET;
    streamServer.sin_addr.s_addr = htonl(INADDR_ANY);
    streamServer.sin_port        = htons(DEFUALT_STREAM_PORT);
    int addr_size = sizeof(struct sockaddr_in);
    nRet = bind(StreamSocket, (struct sockaddr*) &streamServer, addr_size);
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
    UDPWorker();
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
    while (TRUE)
    {
        receiveUDP(SI, streamServer, RecvBytes, Flags);
        Index = WSAWaitForMultipleEvents(1, EventArray, FALSE, WSA_INFINITE, TRUE);

        if (Index == WSA_WAIT_FAILED)
        {
            qDebug() << "WSAWaitForMultipleEvents failed";
            return;
        }
        if (Index != WAIT_IO_COMPLETION)
        {
            // An accept() call event is ready - break the wait loop
            //break;
        }
        if (Index == WAIT_IO_COMPLETION)
        {
            qDebug() << "Server Terminating";
            return;
        }
    }
}
void UDPThreadManager::disconnect()
{
    this->thread()->quit();
}

