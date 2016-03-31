#ifndef GLOBALS_H
#define GLOBALS_H

#include <QVector>
#include <QStringListModel>
#include <winsock2.h>
#include <windows.h>
extern SOCKET listen_socket;
extern SOCKET accept_socket;

extern QVector<std::string> userList;
extern QStringListModel *playlistModel;

class globals
{
public:
    globals();
};



#endif // GLOBALS_H
