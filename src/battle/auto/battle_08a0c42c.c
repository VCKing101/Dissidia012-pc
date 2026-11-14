#include "battle/auto_region2.h"

/* Sets field_08 to 3 when field_0C is 1. */
int func_08A0C42C(Unit0C540 *self)
{
    if (self->field_0C == 1) {
        self->field_08 = 3;
    }
    return 0;
}
