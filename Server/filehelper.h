#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QString>

#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QByteArray>
#include "circlebuff.h"
#include <fstream>
#include <iostream>
#include "networkutility.h"
#include <winsock2.h>
#include <windows.h>

#include "networkutility.h"

#define FILEMAX 60000

class filehelper
{
public:
    filehelper();
    void handleDownloadRequest(QString title, SOCKET m_socket);
    void handleUploadRequest(QString title, SOCKET m_socket);
};

#endif // FILEHELPER_H
