#include "battle/auto_region2.h"

/* Sets field_08 to -1 when field_0C is 1. */
int func_089FBDFC(UnitFBE98 *self)
{
    if (self->field_0C == 1) {
        self->field_08 = -1;
    }
    return 0;
}
