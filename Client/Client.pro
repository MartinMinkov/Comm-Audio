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
    threadmanager.cpp

HEADERS  += client.h \
    threadmanager.h

FORMS    += client.ui

RESOURCES += qdarkstyle/style.qrc

LIBS += -lws2_32
