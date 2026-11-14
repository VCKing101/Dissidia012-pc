#include "battle/auto_region4.h"

/* PartBDE18 deleting destructor. */
void func_08ABD608(PartBDE18 *self, int flags)
{
    if (self) {
        self->vtable = D_08ABDE18;
        func_08ABD670(self);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
