#include "battle/auto_region2.h"

/* UnitFFAE0 constructor. */
UnitFFAE0 *func_089FEE10(UnitFFAE0 *self)
{
    func_08A157D0(self);
    self->vtable = D_089FFAE0;
    func_08A10AC8(&self->field_20);
    func_089F8208(&self->field_4C);
    func_089F5E70(&self->field_84);
    func_08A01788(&self->field_CC);
    func_08A10FA8(&self->field_F4);
    func_08A16758(self->field_108);
    func_089FDE20(&self->field_138);
    func_089FBFD8(&self->field_16C);
    func_089FEF50(self);
    return self;
}
