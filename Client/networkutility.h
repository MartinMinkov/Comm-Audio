#ifndef NETWORKUTILITY_H
#define NETWORKUTILITY_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <QDebug>
#include <sstream>

#define DATA_BUFSIZE 65000
#define PACKET_LEN 1024
#define DEFAULT_VOICE_PORT 8000
#define DEFUALT_STREAM_PORT 9000
#define BUFFSIZE 60000
#define MULTICAST_ADDRESS "234.5.6.7"
#define ERROR_BIT '$'
#define REQ_DOWNLOAD 'v'
#define REQ_UPLOAD "^"
#define REQ_STREAM "!"
#define REQ_CHAT "?"
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
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

extern SOCKET TCPSocket;
extern SOCKET AcceptSocket;
extern SOCKET VCSocket;
extern SOCKET StreamSocket;
extern LPSOCKET_INFORMATION SI;
extern struct sockaddr_in streamServer;
extern QObject * mw;
void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, char* buffer);
void sendDataTCP(SOCKET sd, const char* message);
int WSARead(SOCKET sd, char * message, int timeout, int size);
bool receiveTCP(SOCKET sd, char* message);
void formatMessage(const char* message);
bool WSAS(SOCKET sd, char * message, int size, int timeout);
void sendDatalUDP(LPSOCKET_INFORMATION SI, struct	sockaddr_in server, char* message);
int receiveUDP(LPSOCKET_INFORMATION SI, sockaddr_in server, DWORD RecvBytes, DWORD	Flags);
void CALLBACK ServerRoutine(DWORD Error, DWORD BytesTransferred, LPWSAOVERLAPPED Overlapped, DWORD InFlags);

#endif // NETWORKUTILITY_H
