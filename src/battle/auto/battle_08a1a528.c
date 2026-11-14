#include "battle/auto_region3.h"

/* Unit1B3F8 constructor. */
Unit1B3F8 *func_08A1A528(Unit1B3F8 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A1B3F8;
    func_08A01788(&self->field_1C);
    func_08A18D48(&self->field_44);
    func_08A01DD0(&self->field_68);
    func_08A10AC8(&self->field_8C);
    func_089F8208(&self->field_B8);
    func_089F5E70(&self->field_F0);
    func_08A08178(&self->field_138);
    func_08A1A654(self);
    return self;
}
