#include "battle/auto_region2.h"

/* Unit0CE20 constructor. */
Unit0CE20 *func_08A0C628(Unit0CE20 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A0CE20;
    func_08A01788(&self->field_14);
    func_08A16758(self->field_3C);
    func_08A10AC8(&self->field_6C);
    func_08A0C704(self);
    return self;
}
