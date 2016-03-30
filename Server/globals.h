#ifndef GLOBALS_H
#define GLOBALS_H
#include <winsock2.h>
#include <windows.h>
extern SOCKET listen_socket;
extern SOCKET accept_socket;
class globals
{
public:
    globals();
};

#endif // GLOBALS_H
