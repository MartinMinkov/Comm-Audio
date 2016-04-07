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
public slots:
    void endPlayer();

};

#endif // PLAYERMANAGER_H
