#include "battle/auto_region4.h"

/* PartD9580 constructor: builds the base, then clears the handles. */
PartD9580 *func_08AD7838(PartD9580 *self)
{
    func_08AD29D8(self);
    self->vtable = D_08AD9580;
    self->field_100 = 0;
    self->field_104 = 0;
    self->field_124 = 0;
    self->field_128 = 0;
    self->field_12C = 0;
    self->field_130 = 0;
    self->field_134 = 0;
    return self;
}
