#include "battle/auto_region2.h"

/* Part11298 deleting destructor. */
void func_08A10FE0(Part11298 *self, int flags)
{
    if (self) {
        self->vtable = D_08A11298;
        func_08A1104C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
