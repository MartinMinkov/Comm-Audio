#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QAudioOutput>
#include <QBuffer>
#include <QAudioFormat>
#include <QFile>
#include <QAudio>
#include <QFileDialog>
#include <QDirIterator>

#include "tcpthreadmanager.h"
#include "udpthreadmanager.h"

#include "ui_client.h"
#include "circlebuff.h"
#include "mybuffer.h"
#include "recorder.h"

namespace Ui {
    class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();
    void toggleInput(bool state);


signals:
    void signalStopRecording();


public slots:
    void updateUsers(QVector<QString> userList);
    void updateSongs(QVector<QString> playList);
    void updateCallLabel(QString);
    void handleStateChanged(QAudio::State newState);
    void handleUpdateStatusBar(bool connected);
    Q_SLOT void updateSlider(int percent);
    void setCurrentlyPlaying(QString songName);
    void tabSelected();
    void callNotification();

private slots:
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_updateSongButton_clicked();
    void on_uploadButton_clicked();
    void on_voiceChatButton_clicked();
    void on_downloadSongButton_clicked();
    void on_updateVoiceUsersButton_clicked();
    void on_playStreamButton_clicked();
    void on_rewindStreamButton_clicked();
    void on_stopStreamButton_clicked();
    void on_horizontalSlider_2_sliderReleased();
    void on_liveStreamButton_clicked();
    void on_horizontalSlider_2_sliderPressed();
    void on_updateStreamPlaylist_clicked();
    void on_endChatButton_clicked();
    void on_acceptVoiceButton_clicked();
    void on_button_uploadDirectory_clicked();
    void on_volumeSlider_valueChanged(int value);

    void on_connectedWidget_itemSelectionChanged();

private:
    Ui::client *ui;
    QThread* receiveTCPThread;
    ThreadManager* receiveTCPWorker;

    QThread* receiveUDPThread;
    ThreadManager* receiveUDPWorker;

    QThread* receiveVoiceChatThread;
    ThreadManager* receiveVoiceChatWorker;

    QThread* streamUDPThread;
    UDPThreadManager* streamUDPWorker;

    Recorder rec;
    bool streamSetup;
    bool paused;
};

#endif // CLIENT_H
