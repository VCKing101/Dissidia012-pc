#include "menu/auto_region2.h"

extern unsigned char D_08A45238[];

/* Second screen base constructor. */
ScreenBase2 *func_08A408A0(ScreenBase2 *self)
{
    self->vtable = D_08A45238;
    return self;
}
