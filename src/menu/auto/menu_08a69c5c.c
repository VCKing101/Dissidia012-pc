#include "menu/auto_region2.h"

extern unsigned char D_08A6B170[];

/* Panel6B170 deleting destructor. */
void func_08A69C5C(Panel6B170 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = D_08A6B170;
        func_08A51E08(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
