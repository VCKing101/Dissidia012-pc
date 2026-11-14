#include "battle/auto_region2.h"

/* Stops field_178 and field_A0[1] if they are running; field_08 becomes 2 when func_08A22FE0 reports 0. */
int func_08A142A4(Unit14458 *self)
{
    if (func_089F6360(&self->field_178) == 1) {
        func_089F62E8(&self->field_178);
    }
    if (func_089F8644(&self->field_A0[1]) == 1) {
        func_089F854C(&self->field_A0[1]);
    }
    if (func_08A22FE0(self->field_1F4, self->field_1F8, 0xE) == 0) {
        self->field_08 = 2;
    }
    return 0;
}
