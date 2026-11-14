#include "battle/auto_region1.h"

/* Sets field_08 to -1 when field_0C is 1. */
int func_089FA2A8(UnitFA2C8 *self)
{
    if (self->field_0C == 1) {
        self->field_08 = -1;
    }
    return 0;
}
