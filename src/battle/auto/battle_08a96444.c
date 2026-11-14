#include "battle/auto_region4.h"

/* Part97108 reset. */
void func_08A96444(Part97108 *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_20 = 0;
    self->field_21 = 0;
    func_089A3890(&self->field_30);
    func_089A3890(&self->field_40);
    self->field_50 = 0;
    self->field_51 = 0;
    self->field_52 = 0;
    func_089A3890(&self->field_60);
}
