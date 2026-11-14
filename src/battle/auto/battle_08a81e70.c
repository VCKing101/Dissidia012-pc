#include "battle/auto_region4.h"

/* Part82158 deleting destructor. */
void func_08A81E70(Part82158 *self, int flags)
{
    if (self) {
        self->vtable = D_08A82158;
        func_08A81EDC(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
