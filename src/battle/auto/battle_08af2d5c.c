#include "battle/auto_region4.h"

/* PartFB5A0 deleting destructor: destroys the PartFB558 base. */
void func_08AF2D5C(PartFB5A0 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08AFB5A0;
        func_08AF3E2C(self, 0);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
