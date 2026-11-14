#include "battle/auto_region5.h"

/* Part46080 deleting destructor: destroys the Part44B78 base. */
void func_08B45C7C(Part46080 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08B46080;
        func_08B44BCC(&self->base, 0);
        if (flags & 1) {
            func_08B44D48(self);
        }
    }
}
