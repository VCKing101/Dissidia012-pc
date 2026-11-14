#include "menu/auto_region3.h"

/* ScreenAB7CFC: on the player block's flag 0x800 (with flag 4 or 2), refreshes the five records. */
void func_08AAA980(ScreenAB7CFC *self)
{
    PlayerBlock *block;
    unsigned int flags;
    int i;
    int j;

    block = func_08AABC54();
    flags = self->work->flags;
    if (((flags & 4) != 0) && ((block->flags[0] & 0x800) != 0)) {
        block->flags[0] = block->flags[0] & 0xFFFFF7FF;
        for (i = 0; i < 5; i++) {
            func_08AB16A8(&self->work->records[i]);
        }
        func_08AAA780(self, 1);
        flags = self->work->flags;
    }
    if (((flags & 2) != 0) && ((block->flags[0] & 0x800) != 0)) {
        block->flags[0] = block->flags[0] & 0xFFFFF7FF;
        for (j = 0; j < 5; j++) {
            func_08AB16A8(&self->work->records[j]);
        }
        func_08AAA780(self, 1);
    }
    func_08AAAA8C(self);
}
