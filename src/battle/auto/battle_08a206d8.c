#include "battle/auto_region3.h"

/* Part20B58 deleting destructor. */
void func_08A206D8(Part20B58 *self, int flags)
{
    if (self) {
        self->vtable = D_08A20B58;
        func_08A20764(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
