#include "battle/auto_region2.h"

/* Part0D948 deleting destructor. */
void func_08A0D6F8(Part0D948 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0D948;
        func_08A0D768(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
