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

class Recorder : public QIODevice
{

public:
    Recorder();
    void initializeAudio();

signals:


public slots:
    void stopRecording();

private:
     QAudioFormat format;
     QAudioInput *audioInput;  // class member.
     myBuffer buff;
     QBuffer buffer;

     // QIODevice interface
protected:
     qint64 readData(char *data, qint64 maxlen);
     qint64 writeData(const char *data, qint64 len);
};

#endif // RECORDER_H
