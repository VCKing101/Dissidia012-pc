#include "battle/auto_region5.h"

/* Part2D280 deleting destructor. */
void func_08B2CD08(Part2D280 *self, int flags)
{
    if (self) {
        self->vtable = D_08B2D280;
        func_08B2CE04(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
