#include "battle/auto_region3.h"

/* Part80208 deleting destructor. */
void func_08A800E0(Part80208 *self, int flags)
{
    if (self) {
        self->vtable = D_08A80208;
        func_08A801E4(self);
        func_08A80154(self);
        func_08AAB86C(self->base, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
