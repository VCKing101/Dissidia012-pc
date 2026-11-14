#include "battle/auto_region3.h"

/* Unit2AC40 constructor. */
Unit2AC40 *func_08A2A570(Unit2AC40 *self)
{
    func_089F12F0(&self->base);
    self->base.vtable = D_08A2AC40;
    func_08A10AC8(&self->field_1C);
    func_089F8208(&self->field_48);
    func_089F5E70(&self->field_80);
    func_08A2A64C(self);
    return self;
}
