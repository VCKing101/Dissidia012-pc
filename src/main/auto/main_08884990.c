#include "main/auto_region2.h"

/* Clears flag 0x1. */
void func_08884990(Obj884990 *self)
{
    self->flags = self->flags & 0xfffffffe;
}
