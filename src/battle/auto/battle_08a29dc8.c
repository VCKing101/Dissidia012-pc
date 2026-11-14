#include "battle/auto_region3.h"

/* Unit2A4B8 constructor. */
Unit2A4B8 *func_08A29DC8(Unit2A4B8 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A2A4B8;
    func_08A01788(&self->field_14);
    func_08A16758(&self->field_3C);
    func_08A29E90(self);
    return self;
}
