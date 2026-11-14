#include "battle/auto_region4.h"

/* PartB4240 deleting destructor. */
void func_08AB3CF0(PartB4240 *self, int flags)
{
    if (self) {
        self->vtable = D_08AB4240;
        func_08AB3D80(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
