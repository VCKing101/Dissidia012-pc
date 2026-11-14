#include "menu/auto_region2.h"

/* Screen47D90: sets flag 0x200 when the table's whole range has bit 4. */
void func_08A47130(Screen47D90 *self)
{
    int count;
    unsigned int bits;

    count = func_08A48608(self->work->table);
    bits = func_08A484D8(self->work->table, 0, count - 1);
    if ((bits & 4) != 0) {
        self->work->flags = self->work->flags | 0x200;
    }
}
