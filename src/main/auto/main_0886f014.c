#include "main/auto_region2.h"

/* Clears flag 0x1. */
void func_0886F014(Part86F028 *self)
{
    self->flags = self->flags & 0xfffffffe;
}
