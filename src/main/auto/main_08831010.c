#include "main/auto_region2.h"

/* Deleting destructor of the D_089C7C48 class (base Obj9C6F88). */
void func_08831010(Obj9C6F88 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7C48;
        func_08814360(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
