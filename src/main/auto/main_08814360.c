#include "main/auto_region1.h"

/* Deleting destructor of Obj9C6F88. */
void func_08814360(Obj9C6F88 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C6F88;
        func_088A4EC0(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
