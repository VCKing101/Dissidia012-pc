#include "battle/auto_region3.h"

/* Unit1EB40 constructor. */
Unit1EB40 *func_08A1E1E8(Unit1EB40 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A1EB40;
    func_08A10AC8(&self->field_1C);
    func_089F8208(&self->field_48);
    func_089F5E70(&self->field_80);
    func_08A08178(&self->field_C8);
    func_08A1E2D8(self);
    return self;
}
