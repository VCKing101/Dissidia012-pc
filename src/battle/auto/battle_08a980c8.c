#include "battle/auto_region4.h"

/* Part9A1A0 deleting destructor. */
void func_08A980C8(Part9A1A0 *self, int flags)
{
    if (self) {
        self->vtable = D_08A9A1A0;
        func_08A981C0(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
