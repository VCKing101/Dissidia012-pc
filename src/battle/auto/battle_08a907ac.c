#include "battle/auto_region4.h"

/* Obj90E60 reset. */
void func_08A907AC(Obj90E60 *self)
{
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = -1;
    self->field_10 = 0;
    func_089A3890(&self->field_20);
    self->field_30 = 0;
}
