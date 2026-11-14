#include "battle/auto_region4.h"

/* PartA5610 deleting destructor. */
void func_08AA4CE8(void *object, int flags)
{
    PartA5610 *self = object;

    if (object) {
        self->vtable = D_08AA5610;
        func_08AA4D64(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
