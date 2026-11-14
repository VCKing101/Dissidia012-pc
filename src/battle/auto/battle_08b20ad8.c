#include "battle/auto_region5.h"

/* Part212C8 deleting destructor. */
void func_08B20AD8(Part212C8 *self, int flags)
{
    if (self) {
        self->vtable = D_08B212C8;
        func_08B20B70(self);
        func_0899372C(self->field_0C, 2, sizeof(Part20A68), func_08B1FEA0, 0, 0);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
