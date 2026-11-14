#include "battle/auto_region5.h"

/* Part18810 deleting destructor. */
void func_08B18528(Part18810 *self, int flags)
{
    if (self) {
        self->vtable = D_08B18810;
        func_08B18590(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
