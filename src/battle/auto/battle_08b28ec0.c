#include "battle/auto_region5.h"

/* Part297C8 deleting destructor. */
void func_08B28EC0(Part297C8 *self, int flags)
{
    if (self) {
        self->vtable = D_08B297C8;
        func_08B28F70(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
