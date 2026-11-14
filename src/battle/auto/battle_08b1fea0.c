#include "battle/auto_region5.h"

/* Part20A68 deleting destructor. */
void func_08B1FEA0(Part20A68 *self, int flags)
{
    if (self) {
        self->vtable = D_08B20A68;
        func_08B1FF28(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
