-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T20:23:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app
QT += multimedia

SOURCES += main.cpp\
    server.cpp \
    controlthread.cpp \
    clienthandlerthread.cpp \
    networkutility.cpp \
    filehelper.cpp \
    streamhelper.cpp \
    chathelper.cpp \
    globals.cpp \
    circlebuff.cpp \
    streamhandlerthread.cpp \
    mybuffer.cpp \
    testbuff.cpp \
    playermanager.cpp \
    wavfile.cpp

HEADERS  += server.h \
    controlthread.h \
    clienthandlerthread.h \
    networkutility.h \
    filehelper.h \
    streamhelper.h \
    chathelper.h \
    globals.h \
    circlebuff.h \
    streamhandlerthread.h \
    mybuffer.h \
    testbuff.h \
    playermanager.h \
    wavfile.h

FORMS    += server.ui

RESOURCES += qdarkstyle/style.qrc

LIBS += -lws2_32

CONFIG += c++11
