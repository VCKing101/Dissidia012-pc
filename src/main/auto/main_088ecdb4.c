#include "main/auto_region4.h"

/* Obj9CCB08: deleting destructor. */
void func_088ECDB4(Obj9CCB08 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = &D_089CCB08;
        func_088EC234(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
