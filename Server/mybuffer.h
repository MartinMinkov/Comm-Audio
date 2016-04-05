#ifndef MYBUFFER_H
#define MYBUFFER_H
#include <QDebug>
#include <QAudioOutput>
#include <QIODevice>
#include "globals.h"

#include "networkutility.h"
#include "circlebuff.h"

#define MAXLEN 60000
#define BUFFSIZE 60000

class myBuffer : public QIODevice
{
public:
    myBuffer(int sock);
    circlebuff cData;
    QAudioOutput * player;
    QAudioFormat form;
    QByteArray filler;
    int realPos;

    HANDLE fileReader;
    char buff[BUFFSIZE];
    bool loadSong();
    FILE * testOutput;
    char * loader;
    qint64 readData(char * data, qint64 maxlen);
    qint64 writeData(const char * data, qint64 maxlen);
    qint64 bytesAvailable();
    void getSong(char * songName);
    void startPlayer();
    void setSocket(int socket);


};
DWORD WINAPI readFromFile(LPVOID param);
//bool loadSong();

#endif // MYBUFFER_H
