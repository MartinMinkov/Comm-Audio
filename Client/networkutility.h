#ifndef NETWORKUTILITY_H
#define NETWORKUTILITY_H

#include "globals.h"
#include <ws2tcpip.h>
#include <QDebug>
#include <sstream>

#define DATA_BUFSIZE 65000
#define PACKET_LEN 1024
#define DEFAULT_VOICE_PORT 8000

#define RECEIVE_VOICE_PORT 8001
#define SEND_VOICE_PORT 8002

#define DEFUALT_STREAM_PORT 9000
#define BUFFSIZE 60000
#define MULTICAST_ADDRESS "234.5.6.7"
#define ERROR_BIT '$'
#define REQ_DOWNLOAD 'v'
#define REQ_UPLOAD '^'
#define REQ_STREAM "!"
#define REQ_CHAT "?"
#define REQ_CHAT_IP '@'
#define REQ_REFRESH "%"
#define REFRESH_USER  '&'
#define REFRESH_SONG  '*'

typedef struct _SOCKET_INFORMATION {
    OVERLAPPED	Overlapped;
    SOCKET		Socket;
    CHAR		Buffer[DATA_BUFSIZE];
    WSABUF		DataBuf;
    DWORD		BytesSEND;
    DWORD		BytesRECV;
    struct sockaddr_in server;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

extern bool connectionRequested;
extern SOCKET TCPSocket;
extern SOCKET AcceptSocket;
extern SOCKET VCConnectSocket;
extern SOCKET VCSocket;
extern SOCKET StreamSocket;
extern SOCKET VCRecieveSocket;
extern SOCKET VCSendSocket;

extern LPSOCKET_INFORMATION SI;
extern struct sockaddr_in streamServer;

extern QObject * mw;
void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, char* buffer, struct sockaddr_in server);
void sendDataTCP(SOCKET sd, const char* message);
int WSARead(SOCKET sd, char * message, int timeout, int size);
bool receiveTCP(SOCKET sd, char* message);
void formatMessage(const char* message);
bool WSAS(SOCKET sd, char * message, int size, int timeout);
int receiveUDP(LPSOCKET_INFORMATION SI, sockaddr_in server, DWORD RecvBytes, DWORD	Flags);
void CALLBACK ServerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);

#endif // NETWORKUTILITY_H
