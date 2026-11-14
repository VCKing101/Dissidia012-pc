#include "main/auto_region2.h"

/* Deleting destructor of Obj9C7668. */
void func_088201E8(Obj9C7668 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7668;
        func_08820250(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
