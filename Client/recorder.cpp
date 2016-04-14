#include "recorder.h"

Recorder::Recorder()
{
    point = 40;
    buff = buffer;
    sendOut = false;

    fqt = fopen("testoutok.txt", "wb");
}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	initializeAudio
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	initializeAudio()
--
--
-- RETURNS: VOID
-- NOTES: Set up the microphone reader. Sets format and starts hte player
----------------------------------------------------------------------------------------------------------------------*/
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
    this->open(QIODevice::WriteOnly);
    audioInput = new QAudioInput(format, this);
    //cData.clear();
    audioInput->start(this);
    player = new myBuffer();
    memset(header, '\0', 40);
    strcpy(header, "z-16-44100-2-999-999");

}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	startPlayer
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	startPlayer
--
--
-- RETURNS: VOID
-- NOTES: start the audio output player
----------------------------------------------------------------------------------------------------------------------*/
void Recorder::startPlayer()
{
    //initializeAudio();
    qDebug() << "PLAYER STARTING SECONDARY";
    player->setSocket(1);

}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	startSecondary
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	startSecondary
--
--
-- RETURNS: void
-- NOTES: start audio output player
----------------------------------------------------------------------------------------------------------------------*/
void Recorder::startSecondary(){
    qDebug() << "PLAYER STARTING PLAYER STARTING";
    player->setSocket(1);
}

qint64 Recorder::readData(char *data, qint64 maxlen)
{
    return 0;
}




/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	writedata
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	writeData(const char * data, qint len)
--
--
-- RETURNS: qint64 - number of bytes written
-- NOTES: When data is ready to be written from the microhpone, this is called. Write the data to a buffer, and send
the buffer to the other voice chat person
----------------------------------------------------------------------------------------------------------------------*/
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
        if(player->player->state() == QAudio::IdleState){
            if(cData.headBuff > cData.tail){
                player->resumePlayer();
             qDebug() << "resuming the player";

            }
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


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	stopRecording
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	stopRecording()
--
--
-- RETURNS: VOID
-- NOTES: Stops the mic recording, clears buffer
----------------------------------------------------------------------------------------------------------------------*/

void Recorder::stopRecording()
{
    if (audioInput != NULL)
    {
        cData.clear();
        audioInput->stop();
        player->player->stop();
        delete audioInput;
        audioInput = NULL;
    }
    qDebug() << "Stop Recording";
}
