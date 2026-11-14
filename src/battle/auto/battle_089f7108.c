#include "battle/auto_region1.h"

/* UnitF7B80 constructor. */
UnitF7B80 *func_089F7108(UnitF7B80 *self)
{
    func_08A157D0(self);
    self->vtable = D_089F7B80;
    func_08A10AC8(self->field_1C);
    func_08A08178(self->field_48);
    func_089F8208(&self->field_7C);
    func_089F71E4(self);
    return self;
}
