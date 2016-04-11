#include "recorder.h"

Recorder::Recorder()
{

}
void Recorder::initializeAudio()
{
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qDebug()<<"default format not supported try to use nearest";
        format = info.nearestFormat(format);
    }

    audioInput = new QAudioInput(format, this);
    cData.clear();
    //audioInput->start(buffer);
}

qint64 Recorder::readData(char *data, qint64 maxlen)
{
    return 0;
}

qint64 Recorder::writeData(const char *data, qint64 len)
{
    return 0;
}
void Recorder::stopRecording()
{
    if (audioInput != NULL)
    {
        cData.clear();
        audioInput->stop();
        delete audioInput;
        audioInput = NULL;
    }
    qDebug() << "Stop Recording";
}
