#include "battle/auto_region4.h"

/* PartB7560 deleting destructor. */
void func_08AB7418(PartB7560 *self, int flags)
{
    if (self) {
        self->vtable = D_08AB7560;
        func_08AB7498(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
