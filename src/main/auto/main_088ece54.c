#include "main/auto_region4.h"

/* Obj9CCB48: deleting destructor. */
void func_088ECE54(Obj9CCB48 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = &D_089CCB48;
        func_088EC234(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
