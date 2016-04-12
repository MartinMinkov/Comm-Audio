#ifndef MYBUFFER_H
#define MYBUFFER_H
#include <QDebug>
#include <QAudioOutput>
#include <QIODevice>
#include "circlebuff.h"
#include "networkutility.h"
#include "globals.h"
#define BUFFSIZE 60000

extern circlebuff cData;
class myBuffer : public QIODevice
{
    Q_OBJECT

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
    FILE * fp;

    char * loader;
    bool newSong;
    int currentPos, songTotal, currentTail, songStart;
    qint64 readData(char * data, qint64 maxlen);
    qint64 writeData(const char * data, qint64 maxlen);
    qint64 bytesAvailable();
    void startPlayer();
    void setSocket(int socket);
    void setSlider();

    void pausePlayer();
    void resumePlayer();
    void updateVolume(float v);
    //MAKE THIS SLOT CONNECTED TO LIVE PRESS;
    void jumpLive();
    void sliderChange(int perc);

signals:
    void updateCurrentlyPlaying(QString songName);

};

#endif // MYBUFFER_H
