#include "battle/auto_region4.h"

/* PartA4CA0 deleting destructor. */
void func_08AA42A8(PartA4CA0 *self, int flags)
{
    if (self) {
        self->vtable = D_08AA4CA0;
        func_08AA4328(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
