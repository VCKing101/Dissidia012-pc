#include "main/auto_region2.h"

/* Clears flag 0x1. */
void func_08886640(Obj886208 *self)
{
    self->flags = self->flags & 0xfffffffe;
}
