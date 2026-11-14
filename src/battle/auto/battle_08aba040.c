#include "battle/auto_region4.h"

/* PartBA7C8 deleting destructor. */
void func_08ABA040(PartBA7C8 *self, int flags)
{
    if (self) {
        self->vtable = D_08ABA7C8;
        func_08ABA0B4(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
