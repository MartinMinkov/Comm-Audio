#ifndef NETWORKUTILITY_H
#define NETWORKUTILITY_H

#include <QWidget>
#include <QThread>
#include <QDebug>
#include "controlthread.h"
#include "globals.h"
#define DEFAULT_PORT 7000
#define DATA_BUFSIZE 65000
#define PACKET_LEN 1024

#define REQ_DOWNLOAD 'v'
#define REQ_UPLOAD '^'
#define REQ_STREAM '!'
#define REQ_CHAT '?'
#define REQ_REFRESH '%'
#define REFRESH_USER '&'
#define REFRESH_SONG '*'

#define MAXCONNECTIONS 10


class networkutility
{
private:

public:
    networkutility();

   // void sendDataTCP(SOCKET sd, const char* message);
    bool WSAS(SOCKET sd, char * message, int size, int timeout);
    bool receiveTCP(SOCKET sd, char* message);
    static void debugMessage(const char* message);

};

void sendDataTCP(SOCKET sd, const char* message);
bool receiveTCP(SOCKET sd, char* message);

#endif // NETWORKUTILITY_H
