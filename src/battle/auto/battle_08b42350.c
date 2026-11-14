#include "battle/auto_region5.h"

/* Part42EF8 deleting destructor: destroys the Part44B78 base. */
void func_08B42350(Part42EF8 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08B42EF8;
        func_08B44BCC(&self->base, 0);
        if (flags & 1) {
            func_08B44D48(self);
        }
    }
}
