#include "main/auto_region1.h"

/* Deleting destructor of Obj9C7510. */
void func_0881A204(Obj9C7510 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7510;
        func_0881A284(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
