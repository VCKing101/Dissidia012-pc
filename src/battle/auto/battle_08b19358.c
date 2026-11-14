#include "battle/auto_region5.h"

/* Part19618 deleting destructor. */
void func_08B19358(Part19618 *self, int flags)
{
    if (self) {
        self->vtable = D_08B19618;
        func_08B193C4(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
