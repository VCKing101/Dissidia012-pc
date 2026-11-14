#include "main/auto_region1.h"

/* Deleting destructor of Obj9C74E8. */
void func_08819BC4(Obj9C74E8 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C74E8;
        func_08819C44(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
