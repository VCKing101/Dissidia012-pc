#include "battle/auto_region3.h"

/* Unit18BD8 constructor. */
Unit18BD8 *func_08A17CA8(Unit18BD8 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A18BD8;
    func_08A01788(&self->field_14);
    func_08A18D48(&self->field_3C);
    func_08A01DD0(&self->field_60);
    func_08A16758(&self->field_84);
    func_08A01DD0(&self->field_B4);
    func_08A006A8(&self->field_D8);
    func_08A08178(&self->field_114);
    func_08A17DD4(self);
    return self;
}
