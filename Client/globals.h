#ifndef GLOBALS_H
#define GLOBALS_H
#include <winsock2.h>
#include <windows.h>
#include <QVector>

extern QVector<QString> playlist;
extern QStringList uploadList;
extern QStringList uploadListWithPath;
extern HANDLE dataInBuffer;
extern HANDLE streamStop;
class globals

{
public:
    globals();
};

#endif // GLOBALS_H
