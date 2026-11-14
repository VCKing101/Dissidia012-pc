#include "menu/auto_region1.h"

/* Menu root: whether flag 0x2000 is clear. */
unsigned char func_08A1DEE8(MenuRoot *self)
{
    return (self->work->flags & 0x2000) == 0;
}
