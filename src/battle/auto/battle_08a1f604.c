#include "battle/auto_region3.h"

/* Sets field_08 to -1 when field_0C is 1. */
int func_08A1F604(Unit1F628 *self)
{
    if (self->field_0C == 1) {
        self->field_08 = -1;
    }
    return 0;
}
