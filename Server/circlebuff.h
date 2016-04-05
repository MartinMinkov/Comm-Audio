#ifndef CIRCLEBUFF_H
#define CIRCLEBUFF_H

#include "qplatformdefs.h"
#include <stdlib.h>

#define MAXLEN 1000
#define BLOCKSIZE 60000
class circlebuff
{
public:
    circlebuff();
    int byteLen[MAXLEN];
    char * buff[MAXLEN];
    int head, tail, buffHead;
    void init(){
        int i = 0;
        for(i = 0; i < MAXLEN; i++){
            char * k = new char[BLOCKSIZE];
            buff[i] = k;
            byteLen[i] = 0;
            memset(buff[i], '\0', BLOCKSIZE);
        }
    }

    int pop(char * bff){
        if(tail < head){
            fflush(stdout);
            memcpy(bff, buff[tail % MAXLEN], byteLen[tail % MAXLEN]);
            memset(buff[tail % MAXLEN], '\0', MAXLEN);
            int ret = byteLen[tail % MAXLEN];
            tail++;
            return ret;
        }

        return 0;

    }
    void push(char * add, int len){
        byteLen[head % MAXLEN] = len;
        memcpy(buff[head % MAXLEN], add, len);
        head++;
        buffHead++;
    }
};

#endif // CIRCLEBUFF_H
