#include "battle/auto_region3.h"

/* Part20668 deleting destructor. */
void func_08A20148(Part20668 *self, int flags)
{
    if (self) {
        self->vtable = D_08A20668;
        func_08A201C8(self);
        func_089F7C80(&self->field_04, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
