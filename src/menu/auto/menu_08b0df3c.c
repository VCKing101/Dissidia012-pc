#include "menu/auto_region4.h"

/* ScreenB1CB00: sets up its two cursors (five columns, one row). */
void func_08B0DF3C(ScreenB1CB00 *self)
{
    unsigned int *flags;

    func_088D46E4((MenuCursor *)&self->work->field_24790, 0, 0, 4, 0, -1);
    func_088D46E4((MenuCursor *)&self->work->field_247BC, 0, 0, 1, 0, -1);
    flags = &self->work->field_247BC.flags;
    *flags = *flags | 1;
    func_08B10248(self);
}
