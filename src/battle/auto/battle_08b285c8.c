#include "battle/auto_region5.h"

/* Part28E48 deleting destructor. */
void func_08B285C8(Part28E48 *self, int flags)
{
    if (self) {
        self->vtable = D_08B28E48;
        func_08B28650(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
