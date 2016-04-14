#include "testbuff.h"
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: testbuff.cpp
--
-- FUNCTIONS:
-- testBuff::testBuff(QString songName, QAudioOutput * p)
-- int testBuff::checkWinner()
-- void testBuff::zeroSongList()
-- bool testBuff::loadSong()
-- bool testBuff::setFormat(std::vector<int> vect)
-- qint64 testBuff::writeData(const char * data, qint64 len)
-- qint64 testBuff::readData(char * data, qint64 len)
--
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
--
-- NOTES: A buffer that overwrites QIODevice -- this is handed to a QAudioOutput to be read from
-- OVERLOAD THE READ OR WRITE METHODS
----------------------------------------------------------------------------------------------------------------------*/




QAudioFormat qft;
int pt;
bool nextSong = false;
bool NN = true;
int total;


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: testBuff - constructor
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void testBuff(QString songName, QAudioOutput * p)
--                  QString songName - name of song to play
--                  QAudioOutput * p - player to use
-- RETURNS: CONSTRUCTOR
-- NOTES: Call to create buffer
----------------------------------------------------------------------------------------------------------------------*/
testBuff::testBuff(QString songName, QAudioOutput * p)
{   player = p;
    header = headerBuffer;
    loader = buff;
    packetNum = 0;
    headerLength = 40;

    //get the total song from playlistWithPath
    totalSong = playlistWithPath.size();
    songNumber = 1;
    currentSong = 0;

    this->open(QIODevice::ReadOnly);
    loadSong();


   // qbt = fqt.readAll();
   // fqt.close();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: checkWinner
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: int checkWinner
--
--
-- RETURNS: INT - -1 if no winner, or index of winner in the playlist
-- NOTES: Call to see if clients voted for a song. If they did, return the winning songs index
-- so that the player can load it
----------------------------------------------------------------------------------------------------------------------*/
int testBuff::checkWinner(){
    int max = 0;
    int pos = -1;
    for(int i = 0; i < SLISTSIZE; i++){
        if(songVoting[i] > max){
            max = songVoting[i];
            pos = i;
        }
    }
    printf("Vote for song #: %d", pos);
    return pos;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: zeroSongList
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void zeroSongList

--
--
-- RETURNS: VOID
-- NOTES: Call to zero out the vote array after each song voting has ended
----------------------------------------------------------------------------------------------------------------------*/
void testBuff::zeroSongList(){
    for(int i = 0; i < SLISTSIZE; i++){
        songVoting[i] = 0;
    }
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: loadSong
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	bool loadSong
--
--
-- RETURNS: VOID
-- NOTES: Call to load a song. Checks for a winner - if found plays that song, or else plays the playlist in order.
----------------------------------------------------------------------------------------------------------------------*/
bool testBuff::loadSong(){
    printf("in constructor");
            fflush(stdout);
    nextSong = false;
    QString songNameWithPath;
    QString songName;
    int posNext = checkWinner();
    if(posNext > -1){
        songNameWithPath = playlistWithPath.at(posNext);
        songName = playlist.at(posNext);
        theCurrent = posNext;
    }else{
    songNameWithPath = playlistWithPath.at(currentSong % totalSong);
    songName = playlist.at(currentSong % totalSong);
    theCurrent = currentSong % totalSong;
    }
    zeroSongList();
    currentSong++;
    fqt.setFileName(songNameWithPath);
    //set the currently playing text in server here
    emit triggerUpdateCurrentlyPlayingLabel(songName);

    if(!(fqt.open(QIODevice::ReadOnly))){
        return false;
    }

    qbt = fqt.readAll();
    fileSize = qbt.size();
    fqt.close();
    WavFile wvf;
    wvf.open(songNameWithPath);
    std::vector<int>vect = wvf.getStuff();
    wvf.close();
    getHeader(vect);
    setFormat(vect);

    return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	pauseStream
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void pauseStream
--
--
-- RETURNS: VOID
-- NOTES: puases the audio output player
----------------------------------------------------------------------------------------------------------------------*/
void testBuff::pauseStream(){
    player->suspend();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	reusmeStream
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void resumeStream
--
--
-- RETURNS: VOID
-- NOTES: resumes the audio output
----------------------------------------------------------------------------------------------------------------------*/
void testBuff::resumeStream(){
    player->resume();
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: getHeader
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: void getHeader(std::vector<int> vect)
                        vect - vector of ints holding the wav file format parametegers
--
--
-- RETURNS: VOID
-- NOTES: Pass vector of ints holding sample rate, bits per sample and channels. Adds those parameteger
-- plus file size and song number to a header to be attached to each packet
----------------------------------------------------------------------------------------------------------------------*/
void testBuff::getHeader(std::vector<int> vect){
    QString hold = "";
    int a,b,c;
    a = vect.at(0);
    b = vect.at(1);
    c = vect.at(2);
    char d = songNumber;
    hold += d;
    hold += "-" + QString::number(a);
    hold += "-" + QString::number(b);
    hold += "-" + QString::number(c);
    hold += "-" + QString::number(fileSize/ BUFFSIZE);
    hold += "-" + QString::number(theCurrent % playlist.size()) + "-";
    memset(header, '\0', 40);
    memcpy(header, hold.toStdString().c_str(), 40);
    songNumber++;

    printf(header);
    headLength =  strlen(header);
    fflush(stdout);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: setFormat
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	setFormat(std::vector<int>vect)
--
--
-- RETURNS: BOOL - true = valid format, false = invalid
-- NOTES: Given the sample rate, bits per sample and channels, sets up a new QAudioOutput with those
-- paramters and starts it playing. Called on each new song
----------------------------------------------------------------------------------------------------------------------*/
bool testBuff::setFormat(std::vector<int> vect){

       QAudioFormat format;
       format.setSampleRate(vect.at(1));
       format.setChannelCount(vect.at(2));
       format.setSampleSize(vect.at(0));
       format.setCodec("audio/pcm");
       format.setByteOrder(QAudioFormat::LittleEndian);
       format.setSampleType(QAudioFormat::UnSignedInt);
       QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
       if (!info.isFormatSupported(format)) {
           qWarning() << "Raw audio format not supported by backend, cannot play audio.";
           return false;
       }
       if(player != NULL)
       player->stop();
       player = new QAudioOutput(format, this);
       player->setVolume(0.0);
       currentPos = 0;
       player->start(this);
       return true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	readData
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	qint64 readData(char * data, qint64 len)
--                  char * data - buffer to pass data to
--                  qint64 len  - max amount of data to read
-- RETURNS: QINT64 - number of bytes read
-- NOTES: Called whenever the player needs more data to play. Reads from the circular buffer and
-- copies the data into a buffer to return
----------------------------------------------------------------------------------------------------------------------*/
qint64 testBuff::readData(char * data, qint64 len){
    int length;
    QByteArray chunk;
    fflush(stdout);
    if(NN){
        if(nextSong){
            loadSong();
            return 0;
        }
        currentPos++;
        memcpy(loader, header, 40);
        length = qbt.size();
        if(length > BUFFSIZE){
            chunk = qbt.left(BUFFSIZE - headerLength);
            qbt = qbt.remove(0, BUFFSIZE - headerLength);
            sprintf(loader + headLength, "%d", currentPos);
            memcpy(loader + 40, chunk.toStdString().c_str(), BUFFSIZE - headerLength);

        }else{
            if((len + headerLength) > BUFFSIZE){
                len = BUFFSIZE - headerLength;
            }
            chunk = qbt.left(len);
            qbt = qbt.remove(0, len);
            sprintf(loader + headLength, "%d", currentPos);
            memset(loader + 40, '\0', BUFFSIZE - 40);
            memcpy(loader + 40, chunk.toStdString().c_str(), len);
            nextSong = true;
        }
        total = 0;
        NN = false;
        fflush(stdout);
        sendToMultiCast(loader);
    }
    int remain = BUFFSIZE - total;
    if(remain < len){
        loader += total;
        memcpy(data, loader, remain);
        loader = &buff[0];
        memset(buff, '\0', BUFFSIZE);
        NN = true;
        total = 0;
        return remain;
    }else{
        loader += total;
        memcpy(data, loader + 40, len);
        loader = &buff[0];
        total += len;
        return len;
    }
    //end brace was after return length instead?
    memcpy(data, loader, length);

    return length;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	writeData
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:
--                              NOT USED - MuST OVERWRITE THOUGH
--
-- RETURNS:
-- NOTES:
----------------------------------------------------------------------------------------------------------------------*/
qint64 testBuff::writeData(const char * data, qint64 len){
    return -1;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:
--                              NOT USED -- MST OVERWRITE THOUGH
--
-- RETURNS:
-- NOTES:
----------------------------------------------------------------------------------------------------------------------*/
qint64 testBuff::bytesAvailable(){
    return 0;
}
