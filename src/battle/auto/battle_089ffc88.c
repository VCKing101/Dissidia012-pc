#include "battle/auto_region2.h"

/* Unit005E8 constructor. */
Unit005E8 *func_089FFC88(Unit005E8 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A005E8;
    func_08A01788(&self->field_20);
    func_08A16758(self->field_48);
    func_089F8208(&self->field_78);
    func_089F5E70(&self->field_B0);
    func_089FFD78(self);
    return self;
}
