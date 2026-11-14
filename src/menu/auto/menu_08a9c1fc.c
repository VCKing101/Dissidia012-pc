#include "menu/auto_region3.h"

/* ScreenA3F44: with flag 1 and not 2, hands arg to field_894 and sets flag 2. */
void func_08A9C1FC(ScreenA3F44 *self, unsigned char arg)
{
    unsigned int flags;

    flags = self->work->flags;
    if (((flags & 1) != 0) && ((flags & 2) == 0)) {
        func_08AB4254(&self->field_894, arg);
        self->work->flags = self->work->flags | 2;
    }
}
