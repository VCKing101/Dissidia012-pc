#include "battle/auto_region5.h"

/* Part46668 deleting destructor: destroys the Part44B78 base. */
void func_08B46128(Part46668 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08B46668;
        func_08B44BCC(&self->base, 0);
        if (flags & 1) {
            func_08B44D48(self);
        }
    }
}
