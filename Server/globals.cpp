#include "globals.h"

SOCKET listen_socket = 0;
SOCKET accept_socket = 0;
HANDLE songDone;
globals::globals()
{

}
#include <QVector>

QVector<QString> userList;
QStandardItemModel *playlistModel;
