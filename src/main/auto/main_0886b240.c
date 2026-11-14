#include "main/auto_region2.h"

/* Clears flag 0x1. */
void func_0886B240(ModelObj *self)
{
    self->flags = self->flags & 0xfffffffe;
}
