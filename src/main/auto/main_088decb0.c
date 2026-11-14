#include "main/auto_region4.h"

/* Obj9CC2D0: deleting destructor. */
void func_088DECB0(Obj9CC2D0 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089CC2D0;
        func_088DED28(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
