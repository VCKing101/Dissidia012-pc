#include "battle/auto_region5.h"

/* Part1FCA8 deleting destructor. */
void func_08B1D8E0(Part1FCA8 *self, int flags)
{
    if (self) {
        self->vtable = D_08B1FCA8;
        func_08B1D980(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
