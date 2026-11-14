#include "menu/auto_region4.h"

/* Hands block to D_08B59230 as its request, cleared. */
void func_08B58B64(void *object, void *block)
{
    ((Obj59230X *)object)->request = block;
    ((Request59230 *)block)->flags = 0;
}
