#include "battle/auto_region1.h"

/* PartF8670 reset. */
void func_089F82AC(PartF8670 *self)
{
    self->active = 0;
    self->field_04 = -1;
    self->field_08 = -1;
    self->field_0C = 0;
    self->field_10 = 0;
}
