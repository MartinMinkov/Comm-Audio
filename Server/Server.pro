-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T20:23:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp \
    controlthread.cpp \
    clienthandlerthread.cpp

HEADERS  += server.h \
    controlthread.h \
    clienthandlerthread.h

FORMS    += server.ui

RESOURCES += qdarkstyle/style.qrc

LIBS += -lws2_32
