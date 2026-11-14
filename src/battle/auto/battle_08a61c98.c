#include "battle/auto_region3.h"

/* Unit61CF8 deleting destructor. */
void func_08A61C98(Unit61CF8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A61CF8;
        func_08B2EDC4(self, 0);
        if (flags & 1) {
            func_0883133C(self);
        }
    }
}
