#include "main/auto_region4.h"

/* Obj9CCAD0: deleting destructor. */
void func_088EC8E8(Obj9CCAD0 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = &D_089CCAD0;
        func_088EC234(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
