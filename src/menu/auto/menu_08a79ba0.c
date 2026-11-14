#include "menu/auto_region3.h"

extern unsigned char D_08A79C00[];

/* Step deleting destructor (vtable D_08A79C00). */
void func_08A79BA0(Step *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_08A79C00;
        func_08A775D8(self, 0);
        if ((flags & 1) != 0) {
            func_08992F04(self);
        }
    }
}
