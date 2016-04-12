#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T20:16:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app
QT += multimedia

SOURCES += main.cpp\
        client.cpp \
        networkutility.cpp \
        circlebuff.cpp \
        udpthreadmanager.cpp \
        tcpthreadmanager.cpp \
        mybuffer.cpp \
        globals.cpp \
        recorder.cpp \
    recorderreceive.cpp

HEADERS  += client.h \
    networkutility.h \
    circlebuff.h \
    udpthreadmanager.h \
    tcpthreadmanager.h \
    mybuffer.h \
    globals.h \
    recorder.h \
    recorderreceive.h




FORMS    += client.ui

RESOURCES += qdarkstyle/style.qrc

LIBS += -lws2_32

CONFIG += c++11
