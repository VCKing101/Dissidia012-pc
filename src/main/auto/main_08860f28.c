#include "main/auto_region2.h"

/* Deleting destructor of Obj9C8E60. */
void func_08860F28(Obj9C8E60 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C8E60;
        func_08860FE4(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
