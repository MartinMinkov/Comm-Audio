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

    std::vector<int> getTime(int time);


signals:
    void signalStopRecording();


public slots:
    void updateUsers(QVector<QString> userList);
    void updateSongs(QVector<QString> playList);
    void updateCallLabel(QString);
    void handleStateChanged(QAudio::State newState);
    void handleUpdateStatusBar(bool connected);
    Q_SLOT void updateSlider(float percent, int songTime);
    void setCurrentlyPlaying(QString songName);
    void tabSelected();
    void setDownloadStatus(int state);
    void setUploadStatus(int state);
    void updateIncomingVoiceChatText(QString name);
    void toggleVoiceChatAcceptRejectButtons(bool state);

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
    void on_pushButton_12_clicked();
    void on_downloadFileWidget_itemSelectionChanged();
    void on_uploadFileWidget_itemSelectionChanged();

    void on_streamingPlaylistWidget_itemSelectionChanged();

    void on_rejectVoiceButton_clicked();

private:
    Ui::client *ui;
    QThread* receiveTCPThread;
    ThreadManager* receiveTCPWorker;

    QThread* fileThread;
    ThreadManager* fileWorker;

    QThread* receiveUDPThread;
    ThreadManager* receiveUDPWorker;

    QThread* receiveVoiceChatThread;
    ThreadManager* receiveVoiceChatWorker;

    QThread* streamUDPThread;
    UDPThreadManager* streamUDPWorker;

    Recorder rec;
    bool streamSetup;
    bool paused;
    QString mUsername;
};

#endif // CLIENT_H
