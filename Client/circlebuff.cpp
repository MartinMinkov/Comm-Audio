#include "circlebuff.h"
/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: circlebuff - Circular buffer class.
--
-- FUNCTIONS:
-- init()
-- pop(char * buf)
-- peak(char * buf)
-- push(char * buf)
-- clear()
--
-- DATE:		14/04/2016
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
--
-- NOTES: A circular buffer to store char array data. Provides functions to add and remove data.
----------------------------------------------------------------------------------------------------------------------*/


circlebuff::circlebuff()
{
    tail = 0;
    head = 0;
    headBuff = -2;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: init
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	init()
--
--
-- RETURNS: VOID
-- NOTES: Call to initialize the circular buffer.
----------------------------------------------------------------------------------------------------------------------*/
void circlebuff::init(){
    alive = true;
    int i = 0;
    for(i = 0; i < MAXLEN; i++){
        char * k = new char[BLOCKSIZE];
        buff[i] = k;
        byteLen[i] = 0;
        memset(buff[i], '\0', BLOCKSIZE);
    }
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: pop
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	pop(char * bff)
--
--
-- RETURNS: int - the number of bytes in the current block
-- NOTES: Call to copy the current block into the passed char array
----------------------------------------------------------------------------------------------------------------------*/

int circlebuff::pop(char * bff){
    if(tail < head){
        memcpy(bff, buff[tail % MAXLEN], byteLen[tail % MAXLEN]);
        //memset(buff[tail % MAXLEN], '\0', MAXLEN);
        int ret = byteLen[tail % MAXLEN];
        tail++;
        return ret;
    }

    return 0;

}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: peak
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: int peak(char * bff)
--              char * bff - character array to return data in
--
-- RETURNS: int - number of bytes read.
-- NOTES: Call to copy data into char buffer
----------------------------------------------------------------------------------------------------------------------*/

int circlebuff::peak(char * bff, char c){
    if(tail < head){

        memcpy(bff, buff[tail % MAXLEN], byteLen[tail % MAXLEN]);
        int ret = byteLen[tail % MAXLEN];
        if(bff[0] != c)
            return -1;
        //memset(buff[tail % MAXLEN], '\0', MAXLEN);

        tail++;
        return ret;
    }

    return 0;
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: push
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void push(char * add, int len)
--                      char * add - char array to add to buffer
--                      int len    - length of array to add
-- RETURNS: VOID
-- NOTES: Call to add data to the character buffer
----------------------------------------------------------------------------------------------------------------------*/

void circlebuff::push(char * add, int len){
    byteLen[head % MAXLEN] = len;
    memcpy(buff[head % MAXLEN], add, len);
    head++;
    headBuff++;

    if(headBuff > tail)
        SetEvent(dataInBuffer);

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: del
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE:	void del()
--
--
-- RETURNS: VOID
-- NOTES: deletes the circular buffer
----------------------------------------------------------------------------------------------------------------------*/
void circlebuff::del(){
    for(int i = 0; i < MAXLEN; i++){
        delete(buff[i]);
    }
    alive = false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: clear
-- DATE:	14/04/16
-- REVISIONS:	(V1.0)
-- DESIGNER:	Colin Bose
-- PROGRAMMER:  Colin Bose
-- INTERFACE: void clear()
--
--
-- RETURNS: VOID
-- NOTES: Call to reset the array - zero's out all memory and resets tail and heads
----------------------------------------------------------------------------------------------------------------------*/

void circlebuff::clear(){
    head = 0;
    tail = 0;
    headBuff = -2;
    for(int i = 0; i < MAXLEN; i++){
        memset(buff[i], '\0', BLOCKSIZE);
        byteLen[i] = 0;
    }
}
