#include "main/auto_region2.h"

/* Deleting destructor of Obj9C7680. */
void func_08821110(Obj9C7680 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7680;
        func_0882117C(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
