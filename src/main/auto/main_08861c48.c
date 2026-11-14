#include "main/auto_region2.h"

/* Reset of a step: field_04 = -1, the rest 0. */
void func_08861C48(Step861C48 *self)
{
    self->field_04 = 0xffffffff;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_00 = 0;
}
