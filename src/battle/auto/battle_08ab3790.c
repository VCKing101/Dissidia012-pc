#include "battle/auto_region4.h"

/* PartB3A30 deleting destructor. */
void func_08AB3790(PartB3A30 *self, int flags)
{
    if (self) {
        self->vtable = D_08AB3A30;
        func_08AB37FC(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
