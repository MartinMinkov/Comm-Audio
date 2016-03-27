#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QDebug>
#include <QThread>
#include "threadmanager.h"

#define DATA_BUFSIZE 65000
#define PACKET_LEN 1024

#define REQ_DOWNLOAD v
#define REQ_UPLOAD ^
#define REQ_STREAM !
#define REQ_CHAT ?
#define REQ_REFRESH %

typedef struct _SOCKET_INFORMATION {
    OVERLAPPED	Overlapped;
    SOCKET		Socket;
    CHAR		Buffer[DATA_BUFSIZE];
    WSABUF		DataBuf;
    DWORD		BytesSEND;
    DWORD		BytesRECV;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, const char* buffer);
void sendDataTCP(SOCKET sd, const char* message);
bool receiveTCP(SOCKET sd, char* message);
void formatMessage(const char* message);

namespace Ui {
    class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();


private slots:
    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

private:
    Ui::client *ui;
    QThread* receiveThread;
    ThreadManager* receiveWorker;
};

#endif // CLIENT_H
