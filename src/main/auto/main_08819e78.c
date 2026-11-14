#include "main/auto_region1.h"

/* Deleting destructor of Obj9C74F8. */
void func_08819E78(Obj9C74F8 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C74F8;
        func_08819F00(self);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
