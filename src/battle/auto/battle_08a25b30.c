#include "battle/auto_region3.h"

/* Part26140 deleting destructor. */
void func_08A25B30(Part26140 *self, int flags)
{
    if (self) {
        self->vtable = D_08A26140;
        func_08A25BDC(self);
        func_089F7C80(&self->field_08, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
