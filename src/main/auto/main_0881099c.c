#include "main/auto_region1.h"

/* Deleting destructor of Obj9C6DA0. */
void func_0881099C(Obj9C6DA0 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C6DA0;
        func_08810A04(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
