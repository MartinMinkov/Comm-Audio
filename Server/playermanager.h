#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H
#include <QFile>
#include <QAudioOutput>

class playerManager
{
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
public slots:
    void endPlayer();
private:
    QString mSongName;


};

#endif // PLAYERMANAGER_H
