#include "battle/auto_region2.h"

/* PartFC658 deleting destructor. */
void func_089FC010(PartFC658 *self, int flags)
{
    if (self) {
        self->vtable = D_089FC658;
        func_089FC0D8(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
