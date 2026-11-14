#include "battle/auto_region3.h"

/* When field_0C is 1, calls func_089F62E8(field_80) and sets field_08 to -1. */
int func_08A1FF88(Unit20040 *self)
{
    if (self->field_0C == 1) {
        func_089F62E8(&self->field_80);
        self->field_08 = -1;
    }
    return 0;
}
