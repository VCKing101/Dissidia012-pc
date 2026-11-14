#include "main/auto_region1.h"

/* Deleting destructor of Obj9C74B0. */
void func_08816784(Obj9C74B0 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C74B0;
        func_088167F0(self);
        func_0881AF84(&self->field_04, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
