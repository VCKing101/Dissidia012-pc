#include "battle/auto_region3.h"

/* Part174B8 deleting destructor. */
void func_08A16D88(Part174B8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A174B8;
        func_08A16E50(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
