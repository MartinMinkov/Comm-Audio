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
    void init();

    int pop(char * bff);
    int peak(char * bff, char c);

    void push(char * add, int len);

    void del();

    void clear();
};

#endif // CIRCLEBUFF_H
