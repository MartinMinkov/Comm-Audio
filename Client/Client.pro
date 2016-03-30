#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T20:16:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    networkutility.cpp \
    udpthreadmanager.cpp \
    tcpthreadmanager.cpp \

HEADERS  += client.h \
    networkutility.h \
    udpthreadmanager.h \
    tcpthreadmanager.h \
    networkutility.cpp \
    filemanager.cpp

HEADERS  += client.h \
    networkutility.h \
    filemanager.h

FORMS    += client.ui

RESOURCES += qdarkstyle/style.qrc

LIBS += -lws2_32

CONFIG += c++11
