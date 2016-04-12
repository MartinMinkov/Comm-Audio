#ifndef CIRCLEBUFF_H
#define CIRCLEBUFF_H

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
    void init(){
        int i = 0;
        for(i = 0; i < CIRCBUFFMAXLEN; i++){
            char * k = new char[BLOCKSIZE];
            buff[i] = k;
            byteLen[i] = 0;
            memset(buff[i], '\0', BLOCKSIZE);
        }
    }

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

    void push(char * add, int len){
        byteLen[head % CIRCBUFFMAXLEN] = len;
        memcpy(buff[head % CIRCBUFFMAXLEN], add, len);
        head++;
        buffHead++;
    }
    void clear(){
        head = 0;
        tail = 0;
        headBuff = -2;
        for(int i = 0; i < MAXLEN; i++){
            memset(buff[i], '\0', BLOCKSIZE);
            byteLen[i] = 0;
        }
    }
};

#endif // CIRCLEBUFF_H
