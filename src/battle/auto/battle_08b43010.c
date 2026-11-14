#include "battle/auto_region5.h"

/* Part43668 deleting destructor: destroys the Part44B78 base. */
void func_08B43010(Part43668 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08B43668;
        func_08B44BCC(&self->base, 0);
        if (flags & 1) {
            func_08B44D48(self);
        }
    }
}
