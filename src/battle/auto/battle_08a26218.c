#include "battle/auto_region3.h"

/* Part268C8 deleting destructor. */
void func_08A26218(Part268C8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A268C8;
        func_08A262C4(self);
        func_08A081B8(&self->field_DC, 2);
        func_08A24C80(&self->field_54, 2);
        func_089F8248(&self->field_1C, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
