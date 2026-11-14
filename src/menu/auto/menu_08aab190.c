#include "menu/auto_region3.h"

/* ScreenAB7CFC: with flag 4, counts field_7CE0 down; at 0 sets the player block's flag 0x40000 once. */
void func_08AAB190(ScreenAB7CFC *self)
{
    WorkAB7CFC *work;
    PlayerBlock *block;

    work = self->work;
    if ((work->flags & 4) != 0) {
        work->field_7CE0 = work->field_7CE0 + -1;
        work = self->work;
        if (work->field_7CE0 < 0) {
            work->field_7CE0 = 0;
            work = self->work;
        }
        if ((work->field_7CE0 == 0) && ((work->flags & 0x10) == 0)) {
            block = func_08AABC54();
            block->flags[0] = block->flags[0] | 0x40000;
            self->work->flags = self->work->flags | 0x10;
        }
    }
}
