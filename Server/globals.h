#ifndef GLOBALS_H
#define GLOBALS_H

#include <QVector>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QHeaderView>
#include <winsock2.h>
#include <windows.h>
//#include "clienthandlerthread.h"

extern SOCKET listen_socket;
extern SOCKET accept_socket;

extern QVector<QString> userList;
extern QStandardItemModel *playlistModel;
extern QStringList playlistWithPath;

class globals
{
public:
    globals();
};



#endif // GLOBALS_H
