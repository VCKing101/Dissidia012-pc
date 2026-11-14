#include "menu/auto_region2.h"

/* Marks a layout row empty (its four ids -1). */
void func_08A41454(void *root, void *rows)
{
    ((unsigned short *)rows)[0] = 0xFFFF;
    ((unsigned short *)rows)[1] = 0xFFFF;
    ((unsigned short *)rows)[2] = 0xFFFF;
    ((unsigned short *)rows)[3] = 0xFFFF;
}
