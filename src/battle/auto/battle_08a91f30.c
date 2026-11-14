#include "battle/auto_region4.h"

/* Obj92330 reset. */
void func_08A91F30(Obj92330 *self)
{
    self->field_04 = 0;
    self->field_08 = 0;
    func_089A3890(&self->field_10);
    self->field_20 = 0;
    self->field_24 = 0;
}
