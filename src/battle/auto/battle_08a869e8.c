#include "battle/auto_region4.h"

/* Part86C80 reset. */
void func_08A869E8(Part86C80 *self)
{
    self->active = 0;
    self->field_04 = 0;
    func_089A3890(&self->field_10);
    self->field_20 = 0;
    self->field_24 = 0;
}
