#include "battle/auto_region2.h"

/* Unit097B0 constructor. */
Unit097B0 *func_08A08EA8(Unit097B0 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A097B0;
    func_08A01788(&self->field_14);
    func_08A16758(self->field_3C);
    func_08A088B8(&self->field_6C);
    func_08A083B8(&self->field_9C);
    func_08A08F98(self);
    return self;
}
