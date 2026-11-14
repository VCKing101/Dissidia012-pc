#include "main/auto_region2.h"

/* Deleting destructor of Obj9C9C38. */
void func_08875888(Obj9C9C38 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C9C38;
        func_0887590C(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
