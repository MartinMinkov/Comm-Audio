#include "client.h"
#include "ui_client.h"

client::client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, false);
    }
}

client::~client()
{
    delete ui;
}

void client::on_connectButton_clicked()
{


    QString ipaddr = ui->ipfield->text();
    QString portnum = ui->portfield->text();
    QString username = ui->nameField->text();

    //connect
    receiveThread = new QThread;
    receiveWorker = new ThreadManager();
    receiveWorker->moveToThread(receiveThread);
    //connect(receiveWorker, SIGNAL(connect(ipaddr, portnum, username)), this, SLOT());

    receiveThread->start();

    ui->connectStatus->setText("Connected");
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, true);
    }
}

void initSockInfo(LPSOCKET_INFORMATION SOCKET_INFO, const char* buffer)
{
    /* zero out overlapped structure	*/
    ZeroMemory((&SOCKET_INFO->Overlapped), sizeof(WSAOVERLAPPED));
    SOCKET_INFO->BytesRECV = 0;
    SOCKET_INFO->BytesSEND = 0;
    SOCKET_INFO->DataBuf.len = PACKET_LEN;
    strcpy(SOCKET_INFO->DataBuf.buf, buffer);
}

void sendDataTCP(SOCKET sd, const char* message)
{
    //initSockInfo(SI, message);

    //if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SI->BytesSEND, 0, &(SI->Overlapped), NULL) == SOCKET_ERROR)
    if(send(sd, message, PACKET_LEN, 0) == SOCKET_ERROR)
    {
        /*if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            formatMessage("Sending TCP data information failed");
            //ExitThread(1);
        }*/
    }
    formatMessage("Sending Data to Server");
}

bool receiveTCP(SOCKET sd, char* message)
{
    //if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, &(SI->Overlapped), NULL) == SOCKET_ERROR)
    if(recv(sd, message, PACKET_LEN, 0) == SOCKET_ERROR)
    {
        /*if (WSAGetLastError() != WSA_IO_PENDING)
        {
            //int i = WSAGetLastError();
            return FALSE;
        }*/
        return FALSE;
    }
    return TRUE;
}

void formatMessage(const char* message)
{
    qDebug() << message;
}

void client::on_disconnectButton_clicked()
{
    //disconnect


    ui->connectStatus->setText("Disconnected");
    for(int i= 1; i < 5; i++) {
        ui->tabWidget->setTabEnabled(i, false);
    }
}
