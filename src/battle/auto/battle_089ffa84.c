#include "battle/auto_region2.h"

/* Sets field_08 to -1 once field_CC and field_F4 have both finished. */
int func_089FFA84(UnitFFAE0 *self)
{
    if (func_08A01A9C(&self->field_CC) == 1 && func_08A11270(&self->field_F4) == 1) {
        self->field_08 = -1;
    }
    return 0;
}
