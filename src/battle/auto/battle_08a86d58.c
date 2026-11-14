#include "battle/auto_region4.h"

/* Part870D8 reset. */
void func_08A86D58(Part870D8 *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    func_089A3890(&self->field_10);
    self->field_B0 = -1;
}
