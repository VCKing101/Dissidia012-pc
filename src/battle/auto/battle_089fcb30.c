#include "battle/auto_region2.h"

/* UnitFD348 constructor. */
UnitFD348 *func_089FCB30(UnitFD348 *self)
{
    func_08A157D0(self);
    self->vtable = D_089FD348;
    func_08A01788(&self->field_14);
    func_08A16758(self->field_3C);
    func_089FDE20(&self->field_6C);
    func_089FC668(&self->field_A0);
    func_089FCC20(self);
    return self;
}
