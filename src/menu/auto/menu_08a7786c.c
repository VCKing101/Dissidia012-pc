#include "menu/auto_region3.h"

extern unsigned char D_08A778C8[];

/* Step deleting destructor (vtable D_08A778C8). */
void func_08A7786C(Step *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A778C8;
        func_08A775D8(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
