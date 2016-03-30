/********************************************************************************
** Form generated from reading UI file 'server.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_H
#define UI_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_server
{
public:
    QTextBrowser *testdisplay;
    QGroupBox *groupBox;
    QTextEdit *textEdit;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;
    QListView *playlistView;
    QLabel *label_3;
    QLabel *label_4;
    QPushButton *bAddSongs;
    QGroupBox *groupBox_2;
    QLineEdit *etPort;
    QPushButton *bStartServer;
    QPushButton *bStopServer;

    void setupUi(QWidget *server)
    {
        if (server->objectName().isEmpty())
            server->setObjectName(QStringLiteral("server"));
        server->resize(610, 709);
        testdisplay = new QTextBrowser(server);
        testdisplay->setObjectName(QStringLiteral("testdisplay"));
        testdisplay->setGeometry(QRect(320, 340, 271, 321));
        groupBox = new QGroupBox(server);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(20, 140, 571, 151));
        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(100, 40, 451, 31));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 50, 71, 16));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 80, 91, 41));
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(120, 80, 91, 41));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 130, 131, 16));
        playlistView = new QListView(server);
        playlistView->setObjectName(QStringLiteral("playlistView"));
        playlistView->setGeometry(QRect(20, 340, 281, 321));
        label_3 = new QLabel(server);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 310, 61, 21));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label_3->setFont(font);
        label_4 = new QLabel(server);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(320, 310, 141, 21));
        label_4->setFont(font);
        bAddSongs = new QPushButton(server);
        bAddSongs->setObjectName(QStringLiteral("bAddSongs"));
        bAddSongs->setGeometry(QRect(20, 670, 111, 23));
        groupBox_2 = new QGroupBox(server);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 10, 571, 121));
        etPort = new QLineEdit(groupBox_2);
        etPort->setObjectName(QStringLiteral("etPort"));
        etPort->setGeometry(QRect(20, 30, 61, 20));
        bStartServer = new QPushButton(groupBox_2);
        bStartServer->setObjectName(QStringLiteral("bStartServer"));
        bStartServer->setGeometry(QRect(100, 30, 75, 23));
        bStopServer = new QPushButton(groupBox_2);
        bStopServer->setObjectName(QStringLiteral("bStopServer"));
        bStopServer->setGeometry(QRect(190, 30, 75, 23));

        retranslateUi(server);

        QMetaObject::connectSlotsByName(server);
    } // setupUi

    void retranslateUi(QWidget *server)
    {
        server->setWindowTitle(QApplication::translate("server", "commAudio", 0));
        groupBox->setTitle(QApplication::translate("server", "Stream Controls", 0));
        textEdit->setHtml(QApplication::translate("server", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Stressed Out - Twenty One Pilots</p></body></html>", 0));
        label_2->setText(QApplication::translate("server", "Current Song", 0));
        pushButton->setText(QApplication::translate("server", "Start Stream", 0));
        pushButton_2->setText(QApplication::translate("server", "Stop Stream", 0));
        label->setText(QApplication::translate("server", "Status: Stream stopped", 0));
        label_3->setText(QApplication::translate("server", "Playlist", 0));
        label_4->setText(QApplication::translate("server", "Connected Clients", 0));
        bAddSongs->setText(QApplication::translate("server", "Add Songs", 0));
        groupBox_2->setTitle(QApplication::translate("server", "Server Status", 0));
        etPort->setPlaceholderText(QApplication::translate("server", "Port", 0));
        bStartServer->setText(QApplication::translate("server", "Start Server", 0));
        bStopServer->setText(QApplication::translate("server", "Stop Server", 0));
    } // retranslateUi

};

namespace Ui {
    class server: public Ui_server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_H
