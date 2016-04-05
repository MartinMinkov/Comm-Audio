#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>

#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QByteArray>

#include <fstream>
#include <iostream>

#include <winsock2.h>
#include <windows.h>

#include "networkutility.h"

#define FILEMAX 60000

class filehelper
{
public:
    filehelper();
    void handleDownloadRequest(QString title, SOCKET m_socket);
    void handleUploadRequest();
};

#endif // FILEHELPER_H
