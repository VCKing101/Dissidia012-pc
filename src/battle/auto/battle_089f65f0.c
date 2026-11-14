#include "battle/auto_region1.h"

/* UnitF7040 constructor. */
UnitF7040 *func_089F65F0(UnitF7040 *self)
{
    func_08A157D0(self);
    self->vtable = D_089F7040;
    func_08A10AC8(self->field_1C);
    func_08A08178(self->field_48);
    func_089F8208(&self->field_7C);
    func_089F66CC(self);
    return self;
}
