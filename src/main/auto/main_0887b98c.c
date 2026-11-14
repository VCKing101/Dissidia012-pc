#include "main/auto_region2.h"

/* Clears flag 0x4 and sets flag 0x8. */
void func_0887B98C(Obj87B820 *self)
{
    self->flags = self->flags & 0xfffffffb | 8;
}
