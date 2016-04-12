#include "globals.h"

SOCKET listen_socket = 0;
SOCKET accept_socket = 0;

globals::globals()
{

}

QMap<QString, QString> userListWithIP;
QVector<QString> userList;
QStandardItemModel *playlistModel;
QStringList playlistWithPath;
QStringList playlist;
