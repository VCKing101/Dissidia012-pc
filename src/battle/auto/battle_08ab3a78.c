#include "battle/auto_region4.h"

/* PartB3C90 deleting destructor. */
void func_08AB3A78(PartB3C90 *self, int flags)
{
    if (self) {
        self->vtable = D_08AB3C90;
        func_08AB3AE0(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
