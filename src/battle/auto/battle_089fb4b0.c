#include "battle/auto_region2.h"

/* Releases field_80 and sets field_08 to -1 when field_0C is 1. */
int func_089FB4B0(UnitFB568 *self)
{
    if (self->field_0C == 1) {
        func_089F62E8(&self->field_80);
        self->field_08 = -1;
    }
    return 0;
}
