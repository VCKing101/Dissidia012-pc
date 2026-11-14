#include "battle/auto_region2.h"

/* Stops field_FC if it is running; field_08 becomes 2 when func_08A22FE0 reports 0. */
int func_08A152D0(Unit15448 *self)
{
    if (func_089F6360(&self->field_FC) == 1) {
        func_089F62E8(&self->field_FC);
    }
    if (func_08A22FE0(self->field_178, self->field_17C, 0xD) == 0) {
        self->field_08 = 2;
    }
    return 0;
}
