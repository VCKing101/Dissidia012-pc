#include "battle/auto_region2.h"

/* Unit14458 constructor. */
Unit14458 *func_08A13278(Unit14458 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A14458;
    func_08A01788(&self->field_24);
    func_08A16758(self->field_4C);
    func_08A01DD0(&self->field_7C);
    func_08993560(self->field_A0, 2, sizeof(PartF8670), func_089F8208);
    func_08A12BB8(&self->field_110);
    func_089F5E70(&self->field_178);
    func_08A08178(&self->field_1C0);
    func_08A133C8(self);
    return self;
}
