#include "menu/auto_region2.h"

extern unsigned char D_08A583E0[];

/* Item583E0 deleting destructor. */
void func_08A56A8C(Item583E0 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = D_08A583E0;
        func_08A54F70(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
