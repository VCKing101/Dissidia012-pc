#include "battle/auto_region4.h"

/* PartB7ED0 deleting destructor. */
void func_08AB75A8(PartB7ED0 *self, int flags)
{
    if (self) {
        self->vtable = D_08AB7ED0;
        func_08AB7610(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
