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
    int curSong;
    char buff[BUFFSIZE];
    void setHeader(char * h);
    FILE * testOutput;
    char * loader;
    bool newSong;
    int currentPos, songTotal, currentTail;
    qint64 readData(char * data, qint64 maxlen);
    qint64 writeData(const char * data, qint64 maxlen);
    qint64 bytesAvailable();
    void getSong(char * songName);
    void startPlayer();
    void setSocket(int socket);
    void setSlider();

    //MAKE THIS SLOT CONNECTED TO LIVE PRESS;
    void jumpLive();
    void sliderChange(int perc);



};

#endif // MYBUFFER_H
