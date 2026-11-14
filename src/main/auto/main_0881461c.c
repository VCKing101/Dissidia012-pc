#include "main/auto_region1.h"

/* Deleting destructor of the class with vtable D_089C6FB8 (base Obj9C6F88). */
void func_0881461C(Obj9C6F88 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C6FB8;
        func_08814360(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
