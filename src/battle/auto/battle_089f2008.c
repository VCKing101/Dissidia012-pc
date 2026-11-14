#include "battle/auto_region1.h"

/* PartF2908 constructor. */
PartF2908 *func_089F2008(PartF2908 *self)
{
    self->vtable = D_089F2908;
    func_089F7C48(&self->field_08);
    func_089F7C48(&self->field_28);
    func_089F20C4(self);
    return self;
}
