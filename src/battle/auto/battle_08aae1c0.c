#include "battle/auto_region4.h"

/* PartB2EB8 deleting destructor. */
void func_08AAE1C0(PartB2EB8 *self, int flags)
{
    if (self) {
        self->vtable = D_08AB2EB8;
        func_08AAE22C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
