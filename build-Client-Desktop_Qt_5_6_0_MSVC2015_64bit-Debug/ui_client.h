/********************************************************************************
** Form generated from reading UI file 'client.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENT_H
#define UI_CLIENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_client
{
public:
    QTabWidget *tabWidget;
    QWidget *connectTab;
    QGroupBox *groupBox_13;
    QLineEdit *ipfield;
    QLabel *label_18;
    QLabel *label_19;
    QLineEdit *portfield;
    QLabel *label_22;
    QLineEdit *nameField;
    QGroupBox *groupBox_14;
    QLabel *connectStatus;
    QLabel *label_21;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QWidget *downloadTab;
    QPushButton *pushButton_2;
    QProgressBar *downloadProgress;
    QListWidget *downloadFileWidget;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QListWidget *listWidget_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *updateSongButton;
    QWidget *uploadTab;
    QListWidget *listWidget_4;
    QLabel *label_10;
    QLabel *label_11;
    QPushButton *pushButton_7;
    QLineEdit *lineEdit_2;
    QGroupBox *groupBox_7;
    QLabel *label_12;
    QLabel *label_13;
    QListWidget *listWidget_5;
    QProgressBar *uploadProgress;
    QLabel *label_14;
    QPushButton *uploadButton;
    QWidget *chatTab;
    QGroupBox *groupBox_2;
    QListWidget *connectedWidget;
    QPushButton *pushButton_3;
    QGroupBox *groupBox_3;
    QLabel *label_6;
    QGroupBox *groupBox_4;
    QLabel *label_7;
    QLabel *label_8;
    QGroupBox *groupBox_5;
    QSlider *horizontalSlider;
    QGroupBox *groupBox_6;
    QLabel *label_9;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *voiceChatButton;
    QWidget *streamTab;
    QGroupBox *groupBox_8;
    QPushButton *pushButton_9;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_13;
    QGroupBox *groupBox_9;
    QLineEdit *lineEdit_3;
    QGroupBox *groupBox_10;
    QListWidget *listWidget_6;
    QGroupBox *groupBox_11;
    QLabel *label_17;
    QPushButton *pushButton_12;
    QGroupBox *groupBox_12;
    QSlider *horizontalSlider_3;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_15;
    QSlider *horizontalSlider_2;
    QLabel *label_16;

    void setupUi(QWidget *client)
    {
        if (client->objectName().isEmpty())
            client->setObjectName(QStringLiteral("client"));
        client->resize(739, 388);
        tabWidget = new QTabWidget(client);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 721, 371));
        connectTab = new QWidget();
        connectTab->setObjectName(QStringLiteral("connectTab"));
        groupBox_13 = new QGroupBox(connectTab);
        groupBox_13->setObjectName(QStringLiteral("groupBox_13"));
        groupBox_13->setGeometry(QRect(120, 20, 241, 121));
        ipfield = new QLineEdit(groupBox_13);
        ipfield->setObjectName(QStringLiteral("ipfield"));
        ipfield->setGeometry(QRect(40, 30, 181, 20));
        label_18 = new QLabel(groupBox_13);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(10, 30, 21, 16));
        label_19 = new QLabel(groupBox_13);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(10, 60, 47, 13));
        portfield = new QLineEdit(groupBox_13);
        portfield->setObjectName(QStringLiteral("portfield"));
        portfield->setGeometry(QRect(40, 60, 71, 20));
        label_22 = new QLabel(groupBox_13);
        label_22->setObjectName(QStringLiteral("label_22"));
        label_22->setGeometry(QRect(10, 90, 47, 13));
        nameField = new QLineEdit(groupBox_13);
        nameField->setObjectName(QStringLiteral("nameField"));
        nameField->setGeometry(QRect(40, 90, 113, 20));
        groupBox_14 = new QGroupBox(connectTab);
        groupBox_14->setObjectName(QStringLiteral("groupBox_14"));
        groupBox_14->setGeometry(QRect(370, 20, 201, 121));
        connectStatus = new QLabel(groupBox_14);
        connectStatus->setObjectName(QStringLiteral("connectStatus"));
        connectStatus->setGeometry(QRect(20, 30, 101, 16));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        connectStatus->setFont(font);
        label_21 = new QLabel(groupBox_14);
        label_21->setObjectName(QStringLiteral("label_21"));
        label_21->setGeometry(QRect(20, 50, 101, 16));
        connectButton = new QPushButton(connectTab);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setGeometry(QRect(580, 20, 75, 23));
        disconnectButton = new QPushButton(connectTab);
        disconnectButton->setObjectName(QStringLiteral("disconnectButton"));
        disconnectButton->setGeometry(QRect(580, 50, 75, 23));
        tabWidget->addTab(connectTab, QString());
        downloadTab = new QWidget();
        downloadTab->setObjectName(QStringLiteral("downloadTab"));
        pushButton_2 = new QPushButton(downloadTab);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(240, 280, 201, 23));
        downloadProgress = new QProgressBar(downloadTab);
        downloadProgress->setObjectName(QStringLiteral("downloadProgress"));
        downloadProgress->setGeometry(QRect(102, 311, 341, 21));
        downloadProgress->setValue(24);
        downloadFileWidget = new QListWidget(downloadTab);
        new QListWidgetItem(downloadFileWidget);
        new QListWidgetItem(downloadFileWidget);
        new QListWidgetItem(downloadFileWidget);
        new QListWidgetItem(downloadFileWidget);
        new QListWidgetItem(downloadFileWidget);
        downloadFileWidget->setObjectName(QStringLiteral("downloadFileWidget"));
        downloadFileWidget->setGeometry(QRect(10, 30, 221, 271));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        downloadFileWidget->setFont(font1);
        downloadFileWidget->setAutoFillBackground(false);
        pushButton = new QPushButton(downloadTab);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(240, 190, 201, 23));
        lineEdit = new QLineEdit(downloadTab);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(240, 220, 201, 20));
        label_3 = new QLabel(downloadTab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(11, 311, 85, 16));
        groupBox = new QGroupBox(downloadTab);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(240, 10, 201, 171));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 30, 111, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 50, 111, 16));
        listWidget_3 = new QListWidget(downloadTab);
        listWidget_3->setObjectName(QStringLiteral("listWidget_3"));
        listWidget_3->setGeometry(QRect(450, 30, 256, 301));
        label_4 = new QLabel(downloadTab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 10, 101, 16));
        label_5 = new QLabel(downloadTab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(450, 10, 121, 16));
        updateSongButton = new QPushButton(downloadTab);
        updateSongButton->setObjectName(QStringLiteral("updateSongButton"));
        updateSongButton->setGeometry(QRect(240, 250, 201, 23));
        tabWidget->addTab(downloadTab, QString());
        uploadTab = new QWidget();
        uploadTab->setObjectName(QStringLiteral("uploadTab"));
        listWidget_4 = new QListWidget(uploadTab);
        listWidget_4->setObjectName(QStringLiteral("listWidget_4"));
        listWidget_4->setGeometry(QRect(450, 30, 256, 301));
        label_10 = new QLabel(uploadTab);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 10, 101, 16));
        label_11 = new QLabel(uploadTab);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(450, 10, 121, 16));
        pushButton_7 = new QPushButton(uploadTab);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(240, 190, 201, 23));
        lineEdit_2 = new QLineEdit(uploadTab);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(240, 220, 201, 20));
        groupBox_7 = new QGroupBox(uploadTab);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(240, 10, 201, 171));
        label_12 = new QLabel(groupBox_7);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 30, 111, 16));
        label_13 = new QLabel(groupBox_7);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(10, 50, 111, 16));
        listWidget_5 = new QListWidget(uploadTab);
        new QListWidgetItem(listWidget_5);
        new QListWidgetItem(listWidget_5);
        new QListWidgetItem(listWidget_5);
        new QListWidgetItem(listWidget_5);
        new QListWidgetItem(listWidget_5);
        listWidget_5->setObjectName(QStringLiteral("listWidget_5"));
        listWidget_5->setGeometry(QRect(10, 30, 221, 271));
        listWidget_5->setFont(font1);
        listWidget_5->setAutoFillBackground(false);
        uploadProgress = new QProgressBar(uploadTab);
        uploadProgress->setObjectName(QStringLiteral("uploadProgress"));
        uploadProgress->setGeometry(QRect(102, 311, 341, 21));
        uploadProgress->setValue(24);
        label_14 = new QLabel(uploadTab);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(11, 311, 85, 16));
        uploadButton = new QPushButton(uploadTab);
        uploadButton->setObjectName(QStringLiteral("uploadButton"));
        uploadButton->setGeometry(QRect(240, 280, 201, 23));
        tabWidget->addTab(uploadTab, QString());
        chatTab = new QWidget();
        chatTab->setObjectName(QStringLiteral("chatTab"));
        groupBox_2 = new QGroupBox(chatTab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 151, 321));
        connectedWidget = new QListWidget(groupBox_2);
        new QListWidgetItem(connectedWidget);
        new QListWidgetItem(connectedWidget);
        new QListWidgetItem(connectedWidget);
        new QListWidgetItem(connectedWidget);
        connectedWidget->setObjectName(QStringLiteral("connectedWidget"));
        connectedWidget->setGeometry(QRect(10, 30, 131, 281));
        pushButton_3 = new QPushButton(chatTab);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(170, 120, 201, 31));
        groupBox_3 = new QGroupBox(chatTab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(170, 10, 141, 61));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 30, 101, 16));
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignCenter);
        groupBox_4 = new QGroupBox(chatTab);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(170, 160, 201, 81));
        label_7 = new QLabel(groupBox_4);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 30, 101, 16));
        label_7->setFont(font);
        label_8 = new QLabel(groupBox_4);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 50, 101, 16));
        groupBox_5 = new QGroupBox(chatTab);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(170, 250, 201, 80));
        horizontalSlider = new QSlider(groupBox_5);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(20, 40, 160, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        groupBox_6 = new QGroupBox(chatTab);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(380, 10, 231, 321));
        label_9 = new QLabel(groupBox_6);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 30, 111, 16));
        pushButton_4 = new QPushButton(groupBox_6);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(20, 60, 91, 23));
        pushButton_5 = new QPushButton(groupBox_6);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(120, 60, 91, 23));
        voiceChatButton = new QPushButton(chatTab);
        voiceChatButton->setObjectName(QStringLiteral("voiceChatButton"));
        voiceChatButton->setGeometry(QRect(170, 80, 141, 23));
        tabWidget->addTab(chatTab, QString());
        streamTab = new QWidget();
        streamTab->setObjectName(QStringLiteral("streamTab"));
        groupBox_8 = new QGroupBox(streamTab);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(270, 90, 431, 71));
        pushButton_9 = new QPushButton(groupBox_8);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(20, 30, 91, 31));
        pushButton_10 = new QPushButton(groupBox_8);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(120, 30, 91, 31));
        pushButton_11 = new QPushButton(groupBox_8);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(220, 30, 91, 31));
        pushButton_13 = new QPushButton(groupBox_8);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        pushButton_13->setGeometry(QRect(320, 30, 91, 31));
        groupBox_9 = new QGroupBox(streamTab);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setGeometry(QRect(270, 10, 431, 71));
        lineEdit_3 = new QLineEdit(groupBox_9);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(10, 40, 411, 20));
        groupBox_10 = new QGroupBox(streamTab);
        groupBox_10->setObjectName(QStringLiteral("groupBox_10"));
        groupBox_10->setGeometry(QRect(10, 10, 251, 321));
        listWidget_6 = new QListWidget(groupBox_10);
        listWidget_6->setObjectName(QStringLiteral("listWidget_6"));
        listWidget_6->setGeometry(QRect(10, 30, 231, 281));
        groupBox_11 = new QGroupBox(streamTab);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setGeometry(QRect(270, 240, 231, 91));
        label_17 = new QLabel(groupBox_11);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(10, 30, 241, 16));
        pushButton_12 = new QPushButton(groupBox_11);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        pushButton_12->setGeometry(QRect(10, 50, 75, 23));
        groupBox_12 = new QGroupBox(streamTab);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setGeometry(QRect(510, 239, 191, 91));
        horizontalSlider_3 = new QSlider(groupBox_12);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setGeometry(QRect(20, 40, 160, 22));
        horizontalSlider_3->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(streamTab);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(271, 170, 431, 24));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_15 = new QLabel(layoutWidget);
        label_15->setObjectName(QStringLiteral("label_15"));

        horizontalLayout->addWidget(label_15);

        horizontalSlider_2 = new QSlider(layoutWidget);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider_2);

        label_16 = new QLabel(layoutWidget);
        label_16->setObjectName(QStringLiteral("label_16"));

        horizontalLayout->addWidget(label_16);

        tabWidget->addTab(streamTab, QString());

        retranslateUi(client);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(client);
    } // setupUi

    void retranslateUi(QWidget *client)
    {
        client->setWindowTitle(QApplication::translate("client", "Form", 0));
        groupBox_13->setTitle(QApplication::translate("client", "Server Details", 0));
        label_18->setText(QApplication::translate("client", "IP:", 0));
        label_19->setText(QApplication::translate("client", "Port:", 0));
        label_22->setText(QApplication::translate("client", "Name:", 0));
        groupBox_14->setTitle(QApplication::translate("client", "Status", 0));
        connectStatus->setText(QApplication::translate("client", "Disconnected", 0));
        label_21->setText(QApplication::translate("client", "Duration: 15:02:22", 0));
        connectButton->setText(QApplication::translate("client", "Connect", 0));
        disconnectButton->setText(QApplication::translate("client", "Disconnect", 0));
        tabWidget->setTabText(tabWidget->indexOf(connectTab), QApplication::translate("client", "Connect", 0));
        pushButton_2->setText(QApplication::translate("client", "Download Songs", 0));

        const bool __sortingEnabled = downloadFileWidget->isSortingEnabled();
        downloadFileWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = downloadFileWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("client", "Stressed Out - Twenty One Pilots", 0));
        QListWidgetItem *___qlistwidgetitem1 = downloadFileWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("client", "Sorry - Justin Bieber", 0));
        QListWidgetItem *___qlistwidgetitem2 = downloadFileWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("client", "Renegades - X Ambassadors", 0));
        QListWidgetItem *___qlistwidgetitem3 = downloadFileWidget->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("client", "Hide Away - Daya", 0));
        QListWidgetItem *___qlistwidgetitem4 = downloadFileWidget->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("client", "See You Again - Wiz Khalifa", 0));
        downloadFileWidget->setSortingEnabled(__sortingEnabled);

        pushButton->setText(QApplication::translate("client", "Change Directory", 0));
        lineEdit->setText(QApplication::translate("client", "C:/Downloads", 0));
        label_3->setText(QApplication::translate("client", "Download Status:", 0));
        groupBox->setTitle(QApplication::translate("client", "Files", 0));
        label->setText(QApplication::translate("client", "Number of Songs:", 0));
        label_2->setText(QApplication::translate("client", "Total Size:", 0));
        label_4->setText(QApplication::translate("client", "Select Songs", 0));
        label_5->setText(QApplication::translate("client", "Downloaded Songs", 0));
        updateSongButton->setText(QApplication::translate("client", "Update Song List", 0));
        tabWidget->setTabText(tabWidget->indexOf(downloadTab), QApplication::translate("client", "File Download", 0));
        label_10->setText(QApplication::translate("client", "Select Songs", 0));
        label_11->setText(QApplication::translate("client", "Uploaded Songs", 0));
        pushButton_7->setText(QApplication::translate("client", "Change Directory", 0));
        lineEdit_2->setText(QApplication::translate("client", "C:/Uploads", 0));
        groupBox_7->setTitle(QApplication::translate("client", "Files", 0));
        label_12->setText(QApplication::translate("client", "Number of Songs:", 0));
        label_13->setText(QApplication::translate("client", "Total Size:", 0));

        const bool __sortingEnabled1 = listWidget_5->isSortingEnabled();
        listWidget_5->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem5 = listWidget_5->item(0);
        ___qlistwidgetitem5->setText(QApplication::translate("client", "Stressed Out - Twenty One Pilots", 0));
        QListWidgetItem *___qlistwidgetitem6 = listWidget_5->item(1);
        ___qlistwidgetitem6->setText(QApplication::translate("client", "Sorry - Justin Bieber", 0));
        QListWidgetItem *___qlistwidgetitem7 = listWidget_5->item(2);
        ___qlistwidgetitem7->setText(QApplication::translate("client", "Renegades - X Ambassadors", 0));
        QListWidgetItem *___qlistwidgetitem8 = listWidget_5->item(3);
        ___qlistwidgetitem8->setText(QApplication::translate("client", "Hide Away - Daya", 0));
        QListWidgetItem *___qlistwidgetitem9 = listWidget_5->item(4);
        ___qlistwidgetitem9->setText(QApplication::translate("client", "See You Again - Wiz Khalifa", 0));
        listWidget_5->setSortingEnabled(__sortingEnabled1);

        label_14->setText(QApplication::translate("client", "Upload Status:", 0));
        uploadButton->setText(QApplication::translate("client", "Upload Songs", 0));
        tabWidget->setTabText(tabWidget->indexOf(uploadTab), QApplication::translate("client", "File Upload", 0));
        groupBox_2->setTitle(QApplication::translate("client", "Online Users:", 0));

        const bool __sortingEnabled2 = connectedWidget->isSortingEnabled();
        connectedWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem10 = connectedWidget->item(0);
        ___qlistwidgetitem10->setText(QApplication::translate("client", "Alvin Man", 0));
        QListWidgetItem *___qlistwidgetitem11 = connectedWidget->item(1);
        ___qlistwidgetitem11->setText(QApplication::translate("client", "Colin Bose", 0));
        QListWidgetItem *___qlistwidgetitem12 = connectedWidget->item(2);
        ___qlistwidgetitem12->setText(QApplication::translate("client", "Martin Minkov", 0));
        QListWidgetItem *___qlistwidgetitem13 = connectedWidget->item(3);
        ___qlistwidgetitem13->setText(QApplication::translate("client", "Allen Tsang", 0));
        connectedWidget->setSortingEnabled(__sortingEnabled2);

        pushButton_3->setText(QApplication::translate("client", "End Chat", 0));
        groupBox_3->setTitle(QApplication::translate("client", "Selected User:", 0));
        label_6->setText(QApplication::translate("client", "Martin Minkov", 0));
        groupBox_4->setTitle(QApplication::translate("client", "Status", 0));
        label_7->setText(QApplication::translate("client", "Connected", 0));
        label_8->setText(QApplication::translate("client", "Duration: 15:02:22", 0));
        groupBox_5->setTitle(QApplication::translate("client", "Call volume", 0));
        groupBox_6->setTitle(QApplication::translate("client", "Incoming Voice Chats", 0));
        label_9->setText(QApplication::translate("client", "From: Alvin Man", 0));
        pushButton_4->setText(QApplication::translate("client", "Accept", 0));
        pushButton_5->setText(QApplication::translate("client", "Reject", 0));
        voiceChatButton->setText(QApplication::translate("client", "Call", 0));
        tabWidget->setTabText(tabWidget->indexOf(chatTab), QApplication::translate("client", "Voice Chat", 0));
        groupBox_8->setTitle(QApplication::translate("client", "Stream Controls", 0));
        pushButton_9->setText(QApplication::translate("client", "Play/Pause", 0));
        pushButton_10->setText(QApplication::translate("client", "Stop", 0));
        pushButton_11->setText(QApplication::translate("client", "Rewind", 0));
        pushButton_13->setText(QApplication::translate("client", "Live", 0));
        groupBox_9->setTitle(QApplication::translate("client", "Current Song", 0));
        lineEdit_3->setText(QApplication::translate("client", "Stitches - Shawn Mendes", 0));
        groupBox_10->setTitle(QApplication::translate("client", "Playlist", 0));
        groupBox_11->setTitle(QApplication::translate("client", "Vote for Next Song", 0));
        label_17->setText(QApplication::translate("client", "Selected Song: On My Mind - Ellie Goulding", 0));
        pushButton_12->setText(QApplication::translate("client", "Vote", 0));
        groupBox_12->setTitle(QApplication::translate("client", "Music volume", 0));
        label_15->setText(QApplication::translate("client", "00:02", 0));
        label_16->setText(QApplication::translate("client", "03:40", 0));
        tabWidget->setTabText(tabWidget->indexOf(streamTab), QApplication::translate("client", "Stream Music", 0));
    } // retranslateUi

};

namespace Ui {
    class client: public Ui_client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENT_H
