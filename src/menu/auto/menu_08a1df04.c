#include "menu/auto_region1.h"

/* Menu root: whether flag 8 is set. */
unsigned char func_08A1DF04(MenuRoot *self)
{
    return (self->work->flags & 8) != 0;
}
