#ifndef FILEHELPER_H
#define FILEHELPER_H
#include <QString>
#include <winsock2.h>
#include <windows.h>
class filehelper
{
public:
    filehelper();
    void handleDownloadRequest(QString title, SOCKET m_socket);
    void handleUploadRequest();
};

#endif // FILEHELPER_H
