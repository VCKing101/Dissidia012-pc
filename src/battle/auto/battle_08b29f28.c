#include "battle/auto_region5.h"

/* Part2BAF0 deleting destructor. */
void func_08B29F28(Part2BAF0 *self, int flags)
{
    if (self) {
        self->vtable = D_08B2BAF0;
        func_08B29FF0(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
