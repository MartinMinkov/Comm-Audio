#ifndef NETWORKUTILITY_H
#define NETWORKUTILITY_H

#include <winsock2.h>
#include <windows.h>
#include <QDebug>
#include <sstream>

#define DATA_BUFSIZE 65000
#define PACKET_LEN 1024

#define REQ_DOWNLOAD "v"
#define REQ_UPLOAD "^"
#define REQ_STREAM "!"
#define REQ_CHAT "?"
#define REQ_REFRESH "%"
#define REFRESH_CLIENT  '&'

typedef struct _SOCKET_INFORMATION {
    OVERLAPPED	Overlapped;
    SOCKET		Socket;
    CHAR		Buffer[DATA_BUFSIZE];
    WSABUF		DataBuf;
    DWORD		BytesSEND;
    DWORD		BytesRECV;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

class networkutility
{
public:
    networkutility();
};
void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, const char* buffer);
void sendDataTCP(SOCKET sd, const char* message);
int WSARead(SOCKET sd, char * message, int timeout, int size);
bool receiveTCP(SOCKET sd, char* message);
void formatMessage(const char* message);
bool WSAS(SOCKET sd, char * message, int size, int timeout);

#endif // NETWORKUTILITY_H
