#include "circlebuff.h"

circlebuff::circlebuff()
{
    tail = 0;
    head = 0;
    headBuff = -2;
}
