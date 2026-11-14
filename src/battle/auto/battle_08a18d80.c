#include "battle/auto_region3.h"

/* Part19278 deleting destructor. */
void func_08A18D80(Part19278 *self, int flags)
{
    if (self) {
        self->vtable = D_08A19278;
        func_08A18E18(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
