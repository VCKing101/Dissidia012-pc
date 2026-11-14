#include "battle/auto_region5.h"

/* Part2C228 deleting destructor. */
void func_08B2BB40(Part2C228 *self, int flags)
{
    if (self) {
        self->vtable = D_08B2C228;
        func_08B2BBC0(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
