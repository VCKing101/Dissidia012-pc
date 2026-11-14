#include "battle/auto_region2.h"

/* Part06B08 deleting destructor. */
void func_08A061D0(Part06B08 *self, int flags)
{
    if (self) {
        self->vtable = D_08A06B08;
        func_08A06264(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
