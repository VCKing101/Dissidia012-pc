#include "main/auto_region2.h"

/* Clears actor flag 0x200. */
void func_08833804(ActorView *self)
{
    self->flags_380 = self->flags_380 & 0xfffffdff;
}
