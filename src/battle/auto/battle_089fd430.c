#include "battle/auto_region2.h"

/* UnitFDCA8 constructor. */
UnitFDCA8 *func_089FD430(UnitFDCA8 *self)
{
    func_08A157D0(self);
    self->vtable = D_089FDCA8;
    func_08A01788(&self->field_14);
    func_08A16758(self->field_3C);
    func_089FDE20(&self->field_6C);
    func_089FBFD8(&self->field_A0);
    func_089FD520(self);
    return self;
}
