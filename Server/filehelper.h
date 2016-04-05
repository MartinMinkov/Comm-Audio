#ifndef FILEHELPER_H
#define FILEHELPER_H
#include <QString>

#include <QFile>
#include <QIODevice>
#include <fstream>
#include <QDebug>
#include <iostream>
#include <QByteArray>

#include <winsock2.h>
#include <windows.h>

#include "networkutility.h"

class filehelper
{
public:
    filehelper();
    void handleDownloadRequest(QString title, SOCKET m_socket);
    void handleUploadRequest();
};

#endif // FILEHELPER_H
