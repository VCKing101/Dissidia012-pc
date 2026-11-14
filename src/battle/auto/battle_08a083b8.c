#include "battle/auto_region2.h"

/* Part08880 constructor. */
Part08880 *func_08A083B8(Part08880 *self)
{
    self->vtable = D_08A08880;
    func_089F7C48(&self->field_08);
    func_08A0845C(self);
    return self;
}
