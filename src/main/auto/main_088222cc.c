#include "main/auto_region2.h"

/* Deleting destructor of Obj9C7690. */
void func_088222CC(Obj9C7690 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7690;
        func_088223A8(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
