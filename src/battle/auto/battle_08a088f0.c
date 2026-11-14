#include "battle/auto_region2.h"

/* Part08E98 deleting destructor. */
void func_08A088F0(Part08E98 *self, int flags)
{
    if (self) {
        self->vtable = D_08A08E98;
        func_08A08990(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
