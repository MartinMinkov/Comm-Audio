#include "chathelper.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: chathelper.cpp
--
-- FUNCTIONS:
-- chathelper::chathelper()
-- handleChatRequest
-- handleChatIPRequest(int socket, QString username)
--
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
--
----------------------------------------------------------------------------------------------------------------------*/
chathelper::chathelper()
{

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: handleChatRequest
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: handleChatRequest()
--
-- RETURNS: VOID
-- NOTES: Handles chat request connections
----------------------------------------------------------------------------------------------------------------------*/
void chathelper::handleChatRequest(){
    networkutility::debugMessage("chat requested");
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: handleChatIPRequest
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Alvin Man
-- PROGRAMMER:  Alvin Man
-- INTERFACE: handleChatIPRequest(int socket, QString username)
--
-- RETURNS: VOID
-- NOTES: Logic to handle sending the IP given a certain username.
----------------------------------------------------------------------------------------------------------------------*/
void chathelper::handleChatIPRequest(int socket, QString username){
    //find a matching username in the clientIP table, send it back to client
    if(userListWithIP.contains(username)){
        sendDataTCP(socket, userListWithIP.value(username).toLocal8Bit().data());
    } else {
        qDebug() << "no client with this username found";
    }
}
