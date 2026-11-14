#include "battle/auto_region5.h"

/* Part470C8 deleting destructor: destroys the Part44B78 base. */
void func_08B46AB4(Part470C8 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08B470C8;
        func_08B44BCC(&self->base, 0);
        if (flags & 1) {
            func_08B44D48(self);
        }
    }
}
