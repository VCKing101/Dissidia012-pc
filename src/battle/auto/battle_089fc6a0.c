#include "battle/auto_region2.h"

/* PartFCB00 deleting destructor. */
void func_089FC6A0(PartFCB00 *self, int flags)
{
    if (self) {
        self->vtable = D_089FCB00;
        func_089FC71C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
