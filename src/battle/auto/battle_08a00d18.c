#include "battle/auto_region2.h"

/* Unit01680 constructor. */
Unit01680 *func_08A00D18(Unit01680 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A01680;
    func_08A01788(&self->field_14);
    func_08A16758(self->field_3C);
    func_08A01DD0(&self->field_6C);
    func_08A006A8(&self->field_90);
    func_08A08178(&self->field_CC);
    func_08A00E1C(self);
    return self;
}
