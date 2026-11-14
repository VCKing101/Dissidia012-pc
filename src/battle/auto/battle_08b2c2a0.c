#include "battle/auto_region5.h"

/* Part2CCA8 deleting destructor. */
void func_08B2C2A0(Part2CCA8 *self, int flags)
{
    if (self) {
        self->vtable = D_08B2CCA8;
        func_08B2C334(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
