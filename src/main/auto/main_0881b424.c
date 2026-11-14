#include "main/auto_region1.h"

/* Deleting destructor of Obj9C7530. */
void func_0881B424(Obj9C7530 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7530;
        func_0881B558(self);
        func_08819BC4(&self->field_5C, 2);
        func_0881A204(&self->field_34, 2);
        func_08816784(&self->field_14, 2);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
