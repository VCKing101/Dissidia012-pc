#include "main/auto_region1.h"

/* Deleting destructor of Obj9C74C8. */
void func_08817C7C(Obj9C74C8 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C74C8;
        func_08817D18(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
