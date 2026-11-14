#include "battle/auto_region3.h"

/* Part815C8 deleting destructor. */
void func_08A807A8(Part815C8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A815C8;
        func_08A80884(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
