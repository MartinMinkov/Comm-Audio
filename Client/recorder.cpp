#include "recorder.h"

Recorder::Recorder()
{
    point = 0;
    buff = buffer;
    sendOut = false;

}
void Recorder::initializeAudio()
{
    format.setSampleRate(41000);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format)) {
        qDebug()<<"default format not supported try to use nearest";
        format = info.nearestFormat(format);
    }
    micIn = new micBuffer();
    this->open(QIODevice::WriteOnly);
    audioInput = new QAudioInput(format, this);
    cData.clear();
    audioInput->start(this);
}

qint64 Recorder::readData(char *data, qint64 maxlen)
{
    return 0;
}

qint64 Recorder::writeData(const char *data, qint64 len)
{

    if(point + len > BUFFLEN){
        len = BUFFLEN - point;
        sendOut = true;
    }
    buff += point;
    memcpy(buff, data, len);
    point += len;
    buff = &buffer[0];
    if(sendOut){
        WSAS(VCSocket, buff, BUFFLEN, 100);
        printf("SEnding a packet!");
        fflush(stdout);
        sendOut = false;
        point = 0;
    }
    return len;
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
