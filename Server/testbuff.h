#ifndef TESTBUFF_H
#define TESTBUFF_H
#include <QDebug>
#include <QAudioOutput>
#include <QIODevice>
#include "circlebuff.h"
#include <Windows.h>
#include <QFile>
#include "wavfile.h"
#define BUFFSIZE 60000
class testBuff : public QIODevice
{
public:
    testBuff(QString songName);

    char buff[BUFFSIZE];
    bool loadSong();
    FILE * testOutput;
    QFile fqt;
    char * loader;
    qint64 readData(char * data, qint64 maxlen);
    qint64 writeData(const char * data, qint64 maxlen);
    qint64 bytesAvailable();

    bool publicVariable;
signals:
    void functionNamehere();
};

#endif // TESTBUFF_H
