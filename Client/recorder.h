#ifndef RECORDER_H
#define RECORDER_H

#include <QAudioInput>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QBuffer>
#include "circlebuff.h"
#include "mybuffer.h"
#include "micbuffer.h"
#include "networkutility.h"
#define BUFFLEN 60000
class Recorder : public QIODevice
{

public:
    Recorder();
    void initializeAudio();
    micBuffer * micIn;
    myBuffer * player;
    char * buff;
    char buffer[BUFFLEN];
    bool sendOut;
    FILE * fqt;
    int point;
    char header[40];
    void startSecondary();
signals:
    void signalStartPlayer();

public slots:
    void stopRecording();
    void startPlayer();

private:
     QAudioFormat format;
     QAudioInput *audioInput;  // class member.


     // QIODevice interface
protected:
     qint64 readData(char *data, qint64 maxlen);
     qint64 writeData(const char *data, qint64 len);
};

#endif // RECORDER_H
