#include "main/auto_region1.h"

/* Deleting destructor of Obj9C7520. */
void func_0881AF84(Obj9C7520 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7520;
        func_0881AFF4(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
