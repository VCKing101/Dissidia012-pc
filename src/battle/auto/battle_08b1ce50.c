#include "battle/auto_region5.h"

/* Part1D810 reset. */
void func_08B1CE50(Part1D810 *self)
{
    self->active = 0;
    self->phase = 0;
    self->field_08 = -1;
    self->heap = 0;
    self->field_10 = 0;
    self->field_64 = 0;
    self->field_68 = 0;
}
