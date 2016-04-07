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
    void handleUploadRequest(QString songName, SOCKET m_socket);
};
DWORD WINAPI readStuff(LPVOID param);
#endif // FILEHELPER_H
