#include "main/auto_region2.h"

/* Deleting destructor of Obj9C7650. */
void func_0881F424(Obj9C7650 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7650;
        func_0881F49C(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
