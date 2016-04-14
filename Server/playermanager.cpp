#include "playermanager.h"
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: playermanager.cpp
--
-- FUNCTIONS:
-- void playerManager::startSong(QString songName)
-- void playerManager::getHeader(std::vector<int> vect)
-- bool playerManager::setFormat(std::vector<int> vect)
-- void playerManager::endPlayer()
-- void playerManager::restartStream()
-- void playerManager::relayCurrentlyPlayingSong(QString songName)
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
--
-- NOTES: Manages the audio output player. Has methods to set the format and header for each new song
----------------------------------------------------------------------------------------------------------------------*/

testBuff * t;

playerManager::playerManager()
{
    //LPCWSTR tom = L"test";
    //songDone =  CreateEvent(NULL, TRUE, FALSE,tom );
    songNumber = 1;
    memset(headerBuff, '\0', 40);
    header = headerBuff;

}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startSong
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: startSong(QString songName)
                        QString songName - name of the song to start
--
--
-- RETURNS: VOID
-- NOTES: Opens a song file, gets the length and the header data
----------------------------------------------------------------------------------------------------------------------*/
void playerManager::startSong(QString songName){
    mSongName = songName;
    std::vector<int> vect;
    WavFile wvf;
    qDebug() << songName;
    wvf.open(songName);
    vect = wvf.getStuff();

    wvf.close();

    QFile qft(songName);
    qft.open(QIODevice::ReadOnly);

    fileSize = qft.size();
    qft.close();
    getHeader(vect);
    fflush(stdout);
    setFormat(vect);
    printf("past all the header stuff");
    fflush(stdout);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getHeader
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	getHeader(std::vector<int> vect
--                      vect - vector of ints -- holds the format data
--
--
-- RETURNS: VOID
-- NOTES: Call to format a header. Adds the song number, format information and file size to a header buffer
----------------------------------------------------------------------------------------------------------------------*/
void playerManager::getHeader(std::vector<int> vect){
    QString hold = "";
    int a,b,c;
    a = vect.at(0);
    b = vect.at(1);
    c = vect.at(2);
    char d = songNumber++;
    hold += d;
    hold += "-" + QString::number(a);
    hold += "-" + QString::number(b);
    hold += "-" + QString::number(c);
    hold += "-" + QString::number(fileSize/ BUFFSIZE);

    memcpy(header, hold.toStdString().c_str(), 40);
    printf(header);
    fflush(stdout);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setFormat
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: bool playerManager::setFormat(std::vector<int> vect)
--              vect - vector of ints containing file format
--
-- RETURNS: bool - TRUE = successfully set, FALSE = invlaid format
-- NOTEs: Call to set up a new player
----------------------------------------------------------------------------------------------------------------------*/
bool playerManager::setFormat(std::vector<int> vect){
    QAudioFormat format;

    printf("%d %d %d", vect.at(0), vect.at(1), vect.at(2));
    fflush(stdout);
    format.setSampleRate(66666);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);
    printf("Done setting format");
    fflush(stdout);
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
       qWarning() << "Raw audio format not supported by backend, cannot play audio.";
       return false;
    }
    //QString song("C:/Users/Alvin/Documents/songs/pokemon1.wav");

    t = new testBuff(mSongName, NULL);
    connect(t, SIGNAL(triggerUpdateCurrentlyPlayingLabel(QString)), this, SLOT(relayCurrentlyPlayingSong(QString)), Qt::UniqueConnection);


    //player = new QAudioOutput(format, t);
    //player->start();
    //connect(t, SIGNAL(functionName()), this, SLOT(endPlayer()), Qt::UniqueConnection);

    return true;
}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: startPlayer
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: startPlayer()
--
--
-- RETURNS: VOID
-- NOTES: call to start the player - DEPRECATED
----------------------------------------------------------------------------------------------------------------------*/
void playerManager::startPlayer(){
    //player->start(t);
    //player->setVolume(0.0);

}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: endPlayer
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: endPlayer()
--
--
-- RETURNS: VOID
-- NOTES: CAll to pause the player
----------------------------------------------------------------------------------------------------------------------*/
void playerManager::endPlayer(){
    t->pauseStream();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: restartStream
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: void restartStream
--
--
--
-- RETURNS: VOID
-- NOTES: Call to restart hte stream
----------------------------------------------------------------------------------------------------------------------*/
void playerManager::restartStream(){
    t->resumeStream();
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: relayCurrentlyPlayingSong
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: void playerManager::relayCurrentlyPlayingSong(QString songName)
                            QString songName - name of song to update
--
--
-- RETURNS: VOID
-- NOTES: Call to update the currently playing song label
----------------------------------------------------------------------------------------------------------------------*/
void playerManager::relayCurrentlyPlayingSong(QString songName){
    emit relayCurrentSong(songName);
}
