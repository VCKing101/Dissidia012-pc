#include "battle/auto_region3.h"

/* Once field_54 (a Part254D0) has finished, calls func_089F854C(field_1C)
 * and sets field_04 to -1. */
int func_08A267E8(Part268C8 *self)
{
    if (func_08A25194(&self->field_54) == 1) {
        func_089F854C(&self->field_1C);
        self->field_04 = -1;
    }
    return 0;
}
