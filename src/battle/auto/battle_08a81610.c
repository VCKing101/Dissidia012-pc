#include "battle/auto_region4.h"

/* Part81E28 deleting destructor. */
void func_08A81610(Part81E28 *self, int flags)
{
    if (self) {
        self->vtable = D_08A81E28;
        func_08A8167C(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
