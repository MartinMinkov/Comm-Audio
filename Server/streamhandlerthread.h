#ifndef STREAMHANDLERTHREAD_H
#define STREAMHANDLERTHREAD_H

#include <QObject>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "globals.h"
#include "networkutility.h"

class streamhandlerthread : public QObject
{
    Q_OBJECT
public:
    explicit streamhandlerthread(QObject *parent = 0);


signals:
    void signalInitMutliCast();

public slots:
    void initMultiCast();

private:
    struct ip_mreq stMreq;        /* Multicast interface structure */
};
void sendToMultiCast(char * buff);
#endif // STREAMHANDLERTHREAD_H
