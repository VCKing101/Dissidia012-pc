#include "main/auto_region2.h"

/* Clears flag 0x400. */
void func_0887B944(Obj87B820 *self)
{
    self->flags = self->flags & 0xfffffbff;
}
