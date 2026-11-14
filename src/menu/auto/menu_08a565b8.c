#include "menu/auto_region2.h"

extern unsigned char D_08A583C0[];

/* Item583C0 deleting destructor. */
void func_08A565B8(Item583C0 *self, unsigned int flags)
{
    if (self != 0) {
        self->base.vtable = D_08A583C0;
        func_08A54F70(&self->base, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
