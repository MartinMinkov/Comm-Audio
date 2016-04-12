#ifndef CHATHELPER_H
#define CHATHELPER_H

#include "networkutility.h"

class chathelper
{
public:
    chathelper();
    void handleChatRequest();
    void handleChatIPRequest(int socket, QString username);
};

#endif // CHATHELPER_H
