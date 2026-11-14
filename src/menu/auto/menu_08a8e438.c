#include "menu/auto_region3.h"

/* View8E3B4: -1 unless flag 8; then 1 with flag 0x10, else 0. */
int func_08A8E438(View8E3B4 *self)
{
    int result;
    unsigned int flags;

    flags = self->work->flags;
    result = -1;
    if (((flags & 8) != 0) && (result = 0, (flags & 0x10) != 0)) {
        result = 1;
    }
    return result;
}
