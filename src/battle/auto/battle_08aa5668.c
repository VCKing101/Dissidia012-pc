#include "battle/auto_region4.h"

/* PartA5818 deleting destructor. */
void func_08AA5668(PartA5818 *self, int flags)
{
    if (self) {
        self->vtable = D_08AA5818;
        func_08AA56C8(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
