#include "circlebuff.h"

circlebuff::circlebuff()
{
    tail = 0;
    head = 0;
    headBuff = -2;
}

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

void circlebuff::push(char * add, int len){
    byteLen[head % MAXLEN] = len;
    memcpy(buff[head % MAXLEN], add, len);
    head++;
    headBuff++;

    if(headBuff > tail)
        SetEvent(dataInBuffer);

}

void circlebuff::del(){
    for(int i = 0; i < MAXLEN; i++){
        delete(buff[i]);
    }
    alive = false;
}

void circlebuff::clear(){
    head = 0;
    tail = 0;
    headBuff = -2;
    for(int i = 0; i < MAXLEN; i++){
        memset(buff[i], '\0', BLOCKSIZE);
        byteLen[i] = 0;
    }
}
