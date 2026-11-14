#include "battle/auto_region1.h"

/* UnitF3350 constructor. */
UnitF3350 *func_089F2990(UnitF3350 *self)
{
    func_08A157D0(self);
    self->vtable = D_089F3350;
    func_08A10AC8(self->field_1C);
    func_089F8208(&self->field_48);
    func_089F2008(&self->field_80);
    func_08A08178(self->field_1F0);
    func_089F2A8C(self);
    return self;
}
