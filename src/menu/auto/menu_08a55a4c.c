#include "menu/auto_region2.h"

extern unsigned char D_08A58380[];

/* Item58380 deleting destructor. */
void func_08A55A4C(Item58380 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = D_08A58380;
        func_08A54F70(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
