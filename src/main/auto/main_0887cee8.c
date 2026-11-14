#include "main/auto_region2.h"

/* Deleting destructor of Obj9C9D48. */
void func_0887CEE8(Obj9C9D48 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C9D48;
        func_0887CF80(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
