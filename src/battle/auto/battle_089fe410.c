#include "battle/auto_region2.h"

/* UnitFEC98 constructor. */
UnitFEC98 *func_089FE410(UnitFEC98 *self)
{
    func_08A157D0(self);
    self->vtable = D_089FEC98;
    func_08A01788(&self->field_14);
    func_08A16758(self->field_3C);
    func_089FDE20(&self->field_6C);
    func_089FBFD8(&self->field_A0);
    func_089FE500(self);
    return self;
}
