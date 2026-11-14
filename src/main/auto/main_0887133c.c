#include "main/auto_region2.h"

/* Deleting destructor of Obj9C9680. */
void func_0887133C(Obj9C9680 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C9680;
        func_088713B0(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
