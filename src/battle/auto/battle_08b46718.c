#include "battle/auto_region5.h"

/* Part46A18 deleting destructor: destroys the Part44B78 base. */
void func_08B46718(Part46A18 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08B46A18;
        func_08B44BCC(&self->base, 0);
        if (flags & 1) {
            func_08B44D48(self);
        }
    }
}
