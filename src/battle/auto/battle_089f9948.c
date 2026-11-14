#include "battle/auto_region1.h"

/* UnitFA2C8 constructor. */
UnitFA2C8 *func_089F9948(UnitFA2C8 *self)
{
    func_08A157D0(self);
    self->vtable = D_089FA2C8;
    func_08A01788(self->field_14);
    func_08A16758(self->field_3C);
    func_08A01DD0(self->field_6C);
    func_08A006A8(self->field_90);
    func_08A08178(self->field_CC);
    func_089F9A4C(self);
    return self;
}
