#include "battle/auto_region3.h"

/* Unit20040 constructor. */
Unit20040 *func_08A1F718(Unit20040 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A20040;
    func_08A10AC8(&self->field_1C);
    func_089F8208(&self->field_48);
    func_089F5E70(&self->field_80);
    func_08A08178(&self->field_C8);
    func_08A1F808(self);
    return self;
}
