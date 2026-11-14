#include "battle/auto_region2.h"

/* Unit0FF40 deleting destructor. */
void func_08A0F9BC(Unit0FF40 *self, int flags)
{
    if (self) {
        self->vtable = D_08A0FF40;
        func_08A0FA50(self);
        func_08A0EC18(&self->field_10, 2);
        func_08A157E4(self, 0);
        if (flags & 1) {
            func_08A15834(self);
        }
    }
}
