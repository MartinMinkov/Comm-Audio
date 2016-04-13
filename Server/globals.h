#ifndef GLOBALS_H
#define GLOBALS_H
#define SLISTSIZE 100
#include <QVector>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QHeaderView>
#include <winsock2.h>
#include <windows.h>

extern SOCKET listen_socket;
extern SOCKET accept_socket;
extern int songVoting[SLISTSIZE];
extern QMap<QString, QString> userListWithIP;
extern QVector<QString> userList;
extern QStandardItemModel *playlistModel;
extern QStringList playlistWithPath;
extern QStringList playlist;

class globals
{
public:
    globals();
};



#endif // GLOBALS_H
