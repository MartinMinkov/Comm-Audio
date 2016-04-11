#include "micbuffer.h"

micBuffer::micBuffer()
{
}

qint64 micBuffer::readData(char * data, qint64 maxlen){
    return 0;
}

qint64 micBuffer::writeData(const char * data, qint64 maxlen){
    printf("I WANT TO WRITE STUFF!");
    fflush(stdout);
    return 0;
}

qint64 micBuffer::bytesAvailable(){
    return 0;
}


