#include "battle/auto_region3.h"

/* Part25AD8 deleting destructor. */
void func_08A255F8(Part25AD8 *self, int flags)
{
    if (self) {
        self->vtable = D_08A25AD8;
        func_08A2568C(self);
        func_08A25B30(&self->field_4C, 2);
        func_089F8248(&self->field_14, 2);
        if (flags & 1) {
            func_08992F04(self);
        }
    }
}
