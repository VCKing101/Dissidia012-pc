#include "battle/auto_region3.h"

/* Unit27200 constructor. */
Unit27200 *func_08A268E0(Unit27200 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A27200;
    func_08A08178(&self->field_1C);
    func_089F8208(&self->field_50);
    func_08A269A8(self);
    return self;
}
