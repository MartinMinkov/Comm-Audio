#ifndef MYBUFFER_H
#define MYBUFFER_H

#include <QDebug>
#include <QAudioOutput>
#include <QIODevice>
#include "circlebuff.h"
#include "networkutility.h"

#define BUFFSIZE 60000
extern circlebuff cData;

class myBuffer : public QIODevice
{
public:
    myBuffer();
    QAudioOutput * player;
    QAudioFormat form;
    QByteArray filler;
    int realPos;
    char buff[BUFFSIZE];
    FILE * testOutput;
    char * loader;
    qint64 readData(char * data, qint64 maxlen);
    qint64 writeData(const char * data, qint64 maxlen);
    qint64 bytesAvailable();
    void getSong(char * songName);
    void startPlayer();
    void setSocket(int socket);
    //void fillUp();

};

#endif // MYBUFFER_H
