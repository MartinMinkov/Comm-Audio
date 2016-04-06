#ifndef MYBUFFER_H
#define MYBUFFER_H
#include <QDebug>
#include <QAudioOutput>
#include <QIODevice>
#include "circlebuff.h"
#include <windows.h>
#define BUFFSIZE 60000
class myBuffer : public QIODevice
{
public:
    myBuffer();
    circlebuff cData;
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
   // void fillUp();
    void updateProgress();



};

#endif // MYBUFFER_H
