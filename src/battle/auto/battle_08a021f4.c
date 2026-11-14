#include "battle/auto_region2.h"

/* Unit036D8 deleting destructor. */
void func_08A021F4(Unit036D8 *self, int flags)
{
    if (self) {
        self->base.vtable = D_08A036D8;
        func_08A02288(self);
        func_089F1304(&self->base, 0);
        if (flags & 1) {
            func_089F1354(self);
        }
    }
}
