#include "main/auto_region4.h"

/* Obj9CC2A8: deleting destructor. */
void func_088DE968(Obj9CC2A8 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089CC2A8;
        func_088DE9D8(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
