#include "menu/auto_region3.h"

/* ScreenA3F44: 1 when flag 0x1000 is set with flag 0x80 or 0x40. */
int func_08A9B2D4(ScreenA3F44 *self)
{
    int result;
    unsigned int flags;

    flags = self->work->flags;
    result = 0;
    if ((((flags & 0x80) != 0) || ((flags & 0x40) != 0)) && ((flags & 0x1000) != 0)) {
        result = 1;
    }
    return result;
}
