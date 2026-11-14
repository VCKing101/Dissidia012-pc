#include "menu/auto_region2.h"

extern unsigned char D_08A58400[];

/* Item58400 deleting destructor. */
void func_08A56D0C(Item58400 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = D_08A58400;
        func_08A54F70(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
