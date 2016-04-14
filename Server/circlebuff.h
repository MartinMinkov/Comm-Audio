#ifndef CIRCLEBUFF_H
#define CIRCLEBUFF_H
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


#include "qplatformdefs.h"
#include <stdlib.h>

#define CIRCBUFFMAXLEN 1000
#define BLOCKSIZE 60000
class circlebuff
{
public:
    circlebuff();
    int byteLen[CIRCBUFFMAXLEN];
    char * buff[CIRCBUFFMAXLEN];
    int head, tail, buffHead;
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
    void init(){
        int i = 0;
        for(i = 0; i < CIRCBUFFMAXLEN; i++){
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
    int pop(char * bff){
        if(tail < head){
            fflush(stdout);
            memcpy(bff, buff[tail % CIRCBUFFMAXLEN], byteLen[tail % CIRCBUFFMAXLEN]);
            memset(buff[tail % CIRCBUFFMAXLEN], '\0', CIRCBUFFMAXLEN);
            int ret = byteLen[tail % CIRCBUFFMAXLEN];
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
    int peak(char * bff){
        if(tail < head){
            fflush(stdout);
            memcpy(bff, buff[tail % CIRCBUFFMAXLEN], byteLen[tail % CIRCBUFFMAXLEN]);
            memset(buff[tail % CIRCBUFFMAXLEN], '\0', CIRCBUFFMAXLEN);
            int ret = byteLen[tail % CIRCBUFFMAXLEN];
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
    void push(char * add, int len){
        byteLen[head % CIRCBUFFMAXLEN] = len;
        memcpy(buff[head % CIRCBUFFMAXLEN], add, len);
        head++;
        buffHead++;
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
    void clear(){
        head = 0;
        tail = 0;
        buffHead = -2;
        for(int i = 0; i < CIRCBUFFMAXLEN; i++){
            memset(buff[i], '\0', BLOCKSIZE);
            byteLen[i] = 0;
        }
    }
};

#endif // CIRCLEBUFF_H
