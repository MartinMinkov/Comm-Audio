#include "recorder.h"

Recorder::Recorder()
{
    point = 40;
    buff = buffer;
    sendOut = false;

    fqt = fopen("testoutok.txt", "wb");
}
void Recorder::initializeAudio()
{
    format.setSampleRate(44100);
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
    player = new myBuffer();
    memset(header, '\0', 40);
    strcpy(header, "z-16-44100-2-999-999");

}
void Recorder::startPlayer()
{
    //initializeAudio();
    qDebug() << "PLAYER STARTING SECONDARY";
    player->setSocket(1);

}
void Recorder::startSecondary(){
    qDebug() << "PLAYER STARTING PLAYER STARTING";
    player->setSocket(1);
}

qint64 Recorder::readData(char *data, qint64 maxlen)
{
    return 0;
}


qint64 Recorder::writeData(const char *data, qint64 len)
{

    if(point + len> BUFFLEN){
        len = BUFFLEN - point;
        sendOut = true;
    }
    buff += point;

    memcpy(buff, data, len);
    point += len;
    buff = &buffer[0];
    if(sendOut){
        if(player->player->state() == QAudio::IdleState) {
            if(cData.headBuff > cData.tail)
                player->resumePlayer();
        }

        printf("SEnding a packet!");
        fflush(stdout);
        memcpy(buff, header, 40);

        //WSAS(VCSocket, buff, BUFFLEN, 100);
        if (sendto(VCSendSocket, buff, BUFFLEN, 0, (struct sockaddr *)&voiceChatSend, sizeof(voiceChatSend)) == -1)
        {
            qDebug() << "Sending failed";
        }


        sendOut = false;
        point = 40;
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
