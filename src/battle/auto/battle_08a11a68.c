#include "battle/auto_region2.h"

/* Unit12A88 constructor. */
Unit12A88 *func_08A11A68(Unit12A88 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A12A88;
    func_08A01788(&self->field_1C);
    func_08A16758(self->field_44);
    func_08A01DD0(&self->field_74);
    func_08A10AC8(&self->field_98);
    func_08A10AC8(&self->field_C4);
    func_089F8208(&self->field_F0);
    func_08A11660(&self->field_130);
    func_08A08178(&self->field_280);
    func_08A11BA8(self);
    return self;
}
