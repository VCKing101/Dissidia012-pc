#include "battle/auto_region2.h"

/* Unit0D608 constructor. */
Unit0D608 *func_08A0CEE0(Unit0D608 *self)
{
    func_08A157D0(self);
    self->vtable = D_08A0D608;
    func_08A01788(&self->field_14);
    func_08A16758(self->field_3C);
    func_08A0CFA8(self);
    return self;
}
