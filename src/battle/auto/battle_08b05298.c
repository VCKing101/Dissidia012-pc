#include "battle/auto_region5.h"

/* Part05B88 deleting destructor. */
void func_08B05298(Part05B88 *self, int flags)
{
    if (self) {
        self->vtable = D_08B05B88;
        func_08B0532C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
