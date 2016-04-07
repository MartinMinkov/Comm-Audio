#ifndef MYBUFFER_H
#define MYBUFFER_H
#include <QDebug>
#include <QAudioOutput>
#include <QIODevice>
#include "circlebuff.h"
#include <Windows.h>
#define BUFFSIZE 60000
struct headerHolder{
    int samples;
    int bitsper;
    int channels;
};
class myBuffer : public QIODevice
{

public:

    headerHolder head;
    myBuffer(int sock);
    circlebuff cData;

    QAudioFormat form;
    QByteArray filler;
    int realPos;
    QAudioOutput * player;
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
    void checkHeader(char * header);
    void fillHead(std::vector<int> ok);
    void setFmt();



};

DWORD WINAPI readFromFile(LPVOID param);
//bool loadSong();

#endif // MYBUFFER_H
