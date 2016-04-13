#include "udpthreadmanager.h"
sockaddr_in voiceChatSend;

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
    emit signalUDPWorker(StreamSocket, streamServer);
}
void UDPThreadManager::initalizeVoiceChatSockets(QString ipAddr)
{
    int nRet;
    char flag = 1;

    qDebug() << "In initialize";

    if ((VCRecieveSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
    {
        qDebug() << "Cannot create UDP Receive socket";
        return;
    }
    if ((VCSendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET)
    {
        qDebug() << "Cannot create UDP Send socket";
        return;
    }

    if (connectionRequested == true)
    {
        voiceChatReceive.sin_port        = htons(RECEIVE_VOICE_PORT);
        voiceChatSend.sin_port        = htons(SEND_VOICE_PORT);
    }
    else
    {
        voiceChatReceive.sin_port        = htons(SEND_VOICE_PORT);
        voiceChatSend.sin_port        = htons(RECEIVE_VOICE_PORT);
    }
    voiceChatReceive.sin_family      = AF_INET;
    voiceChatReceive.sin_addr.s_addr = htonl(INADDR_ANY);
    int addr_size = sizeof(struct sockaddr_in);
    nRet = bind(VCRecieveSocket, (struct sockaddr*) &voiceChatReceive, addr_size);
    if (nRet == SOCKET_ERROR)
    {
        qDebug() << "Voice Receieve bind( failed";
        return;
    }
    voiceChatSend.sin_family      = AF_INET;
    voiceChatSend.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((hp = gethostbyname(ipAddr.toStdString().c_str())) == NULL)
    {
        qDebug() << "No host info";
        return;
    }
    memcpy((char *)&voiceChatSend.sin_addr, hp->h_addr, hp->h_length);
    addr_size = sizeof(struct sockaddr_in);

    //Go into receive loop
    qDebug() << "Sending signalUDPWorker";
    UDPWorker(VCRecieveSocket, voiceChatReceive);
}
void UDPThreadManager::UDPWorker(SOCKET sd, struct sockaddr_in socketStruct)
{
    DWORD RecvBytes = 0, Index;
    DWORD Flags = 0;
    WSAEVENT				UDPEvent;
    WSAEVENT				EventArray[1  ];

    qDebug() << "In UDPWorker";
    //Creating Socket Info struct
    if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
    {
        qDebug() << "GlobalAlloc() failed with error";
        return;
    }
    qDebug() << "Before UDPEvent";
    if ((UDPEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
    {
        qDebug() << "WSACreateEvent() failed";
        return;
    }
    qDebug() << "After UDPEvent";

    // Save the event in the event array.
    EventArray[0] = UDPEvent;

    //Copy socket
    SI->Socket = sd;
    SI->server = socketStruct;
    // Fill in the details of our socket.
    initSockInfo(SI, SI->Buffer, SI->server);
    receiveUDP(SI, socketStruct, RecvBytes, Flags);
    qDebug() << "Before while loop";
    while (TRUE)
    {
        Index = WSAWaitForMultipleEvents(1, EventArray, FALSE, WSA_INFINITE, TRUE);
        if (Index == WSA_WAIT_FAILED)
        {
            qDebug() << "WSAWaitForMultipleEvents failed";
            return;
        }
        if (Index != WAIT_IO_COMPLETION)
        {
            // An accept() call event is ready - break the wait loop
            return;
        }
        if (Index == WAIT_IO_COMPLETION)
        {
            continue;
        }
    }
}
void UDPThreadManager::disconnect()
{
    this->thread()->quit();
}

