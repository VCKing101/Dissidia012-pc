#include "battle/auto_region5.h"

/* Part46080 constructor: builds the Part44B78 base. */
Part46080 *func_08B45C40(Part46080 *self)
{
    func_08B44B78(&self->base);
    self->base.vtable = D_08B46080;
    self->field_30 = 0;
    self->field_34 = 0;
    return self;
}
