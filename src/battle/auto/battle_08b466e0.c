#include "battle/auto_region5.h"

/* Part46A18 constructor: builds the Part44B78 base. */
Part46A18 *func_08B466E0(Part46A18 *self)
{
    func_08B44B78(&self->base);
    self->base.vtable = D_08B46A18;
    self->field_30 = 0;
    return self;
}
