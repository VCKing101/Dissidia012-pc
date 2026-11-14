#include "battle/auto_region2.h"

/* Stops field_F0 if it is running; field_08 becomes 2 when func_08A22FE0 reports 0. */
int func_08A128D8(Unit12A88 *self)
{
    if (func_089F8644(&self->field_F0) == 1) {
        func_089F854C(&self->field_F0);
    }
    if (func_08A22FE0(self->field_2B4, self->field_2B8, 0xD) == 0) {
        self->field_08 = 2;
    }
    return 0;
}
