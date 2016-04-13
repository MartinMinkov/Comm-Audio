#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H
#include <QFile>
#include <QObject>
#include <QAudioOutput>

#include "testbuff.h"

class playerManager : public QObject
{
    Q_OBJECT
public:
    QAudioOutput * player;
    //HANDLE fileReader;
    QFile fqt;
    playerManager();
    bool setFormat(std::vector<int>);
    void startSong(QString songName);
    void startPlayer();
    int songNumber;
    int fileSize;
    char * header;
    char headerBuff[100];
    void getHeader(std::vector<int> vect);
    void restartStream();
public slots:
    void endPlayer();
    void relayCurrentlyPlayingSong(QString songName);
private:
    QString mSongName;
signals:
    void relayCurrentSong(QString songName);


};

#endif // PLAYERMANAGER_H
