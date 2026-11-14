#include "battle/auto_region4.h"

/* PartB97D0 deleting destructor. */
void func_08AB88A0(PartB97D0 *self, int flags)
{
    if (self) {
        self->vtable = D_08AB97D0;
        func_08AB8908(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
