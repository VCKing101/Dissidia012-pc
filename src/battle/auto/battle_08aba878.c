#include "battle/auto_region4.h"

/* PartBAC98 reset. */
void func_08ABA878(PartBAC98 *self)
{
    int i;

    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    for (i = 0; i < 2; i++) {
        self->field_10[i] = 0;
        self->field_18[i] = 0;
    }
}
