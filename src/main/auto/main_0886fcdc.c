#include "main/auto_region2.h"

/* func_088716C8 on D_089D5FF0 rounded up to 8 KB. */
unsigned int func_0886FCDC(void)
{
    int size;

    size = func_088716C8(D_089D5FF0);
    return size + 0x1fffU & 0xffffe000;
}
