#ifndef CIRCLEBUFF_H
#define CIRCLEBUFF_H

#include "qplatformdefs.h"
#include <stdlib.h>

#define MAXLEN 500
#define BLOCKSIZE 60000

class circlebuff
{
public:
    circlebuff();
    int byteLen[MAXLEN];
    char * buff[MAXLEN];
    int head, tail, headBuff;
    bool alive;
    void init(){
        alive = true;
        int i = 0;
        for(i = 0; i < MAXLEN; i++){
            char * k = new char[BLOCKSIZE];
            buff[i] = k;
            byteLen[i] = 0;
            memset(buff[i], '/0', BLOCKSIZE);
        }
    }

    int pop(char * bff){
        if(tail < head){
            memcpy(bff, buff[tail % MAXLEN], byteLen[tail % MAXLEN]);
//            memset(buff[tail % MAXLEN], '\0', MAXLEN);
            int ret = byteLen[tail % MAXLEN];
            tail++;
            return ret;
        }

        return 0;

    }
    int peak(char * bff, char c){
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

    void push(char * add, int len){
        byteLen[head % MAXLEN] = len;
        memcpy(buff[head % MAXLEN], add, len);
        head++;
        headBuff++;
    }

    void del(){
        for(int i = 0; i < MAXLEN; i++){
            delete(buff[i]);
        }
        alive = false;
    }

    void clear(){
        head = 0;
        tail = 0;
        headBuff = -10;
        for(int i = 0; i < MAXLEN; i++){
            memset(buff[i], '\0', BLOCKSIZE);
        }
    }
};

#endif // CIRCLEBUFF_H
