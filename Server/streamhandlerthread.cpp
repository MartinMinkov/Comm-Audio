#include "streamhandlerthread.h"
#include "networkutility.h"
#define MARTIN 60000

streamhandlerthread::streamhandlerthread(QObject *parent) : QObject(parent)
{

}
void streamhandlerthread::initMultiCast()
{
    qDebug() << "Initate multi cast";
    int nRet;
    u_long lTTL = TIMECAST_TTL;
    bool fFlag;

    StreamSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (StreamSocket == INVALID_SOCKET)
    {
      qDebug() << "socket() failed";
      return;
    }
    stLclAddr.sin_family      = AF_INET;
    stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);                  /* any interface */
    stLclAddr.sin_port        = DEFUALT_STREAM_PORT;                 /* any port */
    nRet = bind(StreamSocket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));
    if (nRet == SOCKET_ERROR)
    {
        qDebug() << "bind() failed";
        return;
    }
    stMreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS);
    stMreq.imr_interface.s_addr = INADDR_ANY;
    nRet = setsockopt(StreamSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
    if (nRet == SOCKET_ERROR)
    {
      qDebug() << "setsockopt() IP_ADD_MEMBERSHIP address failed";
      return;
    }

    /* Set IP TTL to traverse up to multiple routers */
    nRet = setsockopt(StreamSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&lTTL, sizeof(lTTL));
    if (nRet == SOCKET_ERROR) {
        qDebug() << "setsockopt() IP_MULTICAST_TTL failed";
        return;
    }

    /* Disable loopback */
    fFlag = FALSE;
    nRet = setsockopt(StreamSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&fFlag, sizeof(fFlag));
    if (nRet == SOCKET_ERROR)
    {
        qDebug() << "setsockopt() IP_MULTICAST_LOOP failed";
        return;
    }
    /* Assign our destination address */
    client.sin_family =      AF_INET;
    client.sin_addr.s_addr = inet_addr(MULTICAST_ADDRESS);
    client.sin_port =        htons(DEFUALT_STREAM_PORT);

    qDebug() << "Attempting to send to multicast";
    //Send stuff
//    sendToMultiCast();
}
void sendToMultiCast(char * buff)
{
    int nRet;
    char buf[PACKET_LEN];
    strcpy(buf, "eyybyy");
    //while (1)
    //{
    printf("Sending data");
        nRet = sendto(StreamSocket, buff, MARTIN, 0, (struct sockaddr*)&client, sizeof(client));
        if (nRet < 0)
        {   nRet = GetLastError();
            qDebug() << "sendto() failed";
            return;
      //  }
       // Sleep(1000);
    }
}
