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
