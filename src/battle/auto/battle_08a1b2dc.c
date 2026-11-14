#include "battle/auto_region3.h"

/* Stops field_F0 and sets field_08 to -1. */
int func_08A1B2DC(Unit1B3F8 *self)
{
    func_089F62E8(&self->field_F0);
    self->field_08 = -1;
    return 0;
}
