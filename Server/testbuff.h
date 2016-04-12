#ifndef TESTBUFF_H
#define TESTBUFF_H
#include <QDebug>
#include <QObject>
#include <QAudioOutput>
#include <QIODevice>
#include <QFile>
#include "circlebuff.h"
//#include "wavfile.h"
#include "globals.h"
#include "streamhandlerthread.h"
#include "wavfile.h"

#define BUFFSIZE 60000

class testBuff : public QIODevice
{
    Q_OBJECT
public:
    testBuff(QString songName, QAudioOutput * p);
    QByteArray qbt;
    int headerLength;
    int currentPos;
    int packetNum;
    char buff[BUFFSIZE];
    bool loadSong();
    FILE * testOutput;
    QFile fqt;
    int headLength;
    QAudioOutput * player;
    char * loader;
    int fileSize;
    int songNumber;
    qint64 readData(char * data, qint64 maxlen);
    qint64 writeData(const char * data, qint64 maxlen);
    bool setFormat(std::vector<int> vect);
    void getHeader(std::vector<int> vect);
    qint64 bytesAvailable();
    char * header;
    int currentSong, totalSong;
    QStringList playList;
    char headerBuffer[40];
    void createHeader(char * head);
    bool publicVariable;
signals:
    void triggerUpdateCurrentlyPlayingLabel(QString songName);
};

#endif // TESTBUFF_H
