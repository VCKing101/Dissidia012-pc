#include "battle/auto_region4.h"

/* PartC3C00 deleting destructor. */
void func_08AC1EB0(PartC3C00 *self, int flags)
{
    if (self) {
        self->vtable = D_08AC3C00;
        func_08AC1F48(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
