#include "battle/auto_region3.h"

/* Part21F38 deleting destructor. */
void func_08A21C78(Part21F38 *self, int flags)
{
    if (self) {
        self->vtable = D_08A21F38;
        func_08A21CE4(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
