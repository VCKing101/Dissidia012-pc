#include "battle/auto_region3.h"

/* Unit213C0 constructor. */
Unit213C0 *func_08A20B90(Unit213C0 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A213C0;
    func_08A01788(&self->field_14);
    func_08A16758(&self->field_3C);
    func_08A01DD0(&self->field_6C);
    func_08A20108(&self->field_90);
    func_08A206A0(&self->field_C8);
    func_08A20C94(self);
    return self;
}
