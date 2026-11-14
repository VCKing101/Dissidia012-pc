#include "battle/auto_region2.h"

/* Unit15448 constructor. */
Unit15448 *func_08A145E0(Unit15448 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A15448;
    func_08A01788(&self->field_1C);
    func_08A16758(self->field_44);
    func_08A01DD0(&self->field_74);
    func_08A10AC8(&self->field_98);
    func_089F8208(&self->field_C4);
    func_089F5E70(&self->field_FC);
    func_08A08178(&self->field_144);
    func_08A1470C(self);
    return self;
}
