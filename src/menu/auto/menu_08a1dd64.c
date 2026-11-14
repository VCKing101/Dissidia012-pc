#include "menu/auto_region1.h"

/* Menu root: whether flag 4 is set. */
unsigned char func_08A1DD64(MenuRoot *self)
{
    return (self->work->flags & 4) != 0;
}
