#include "battle/auto_region5.h"

/* Part46668 constructor: builds the Part44B78 base. */
Part46668 *func_08B460E8(Part46668 *self)
{
    func_08B44B78(&self->base);
    self->base.vtable = D_08B46668;
    self->field_40 = 0;
    self->field_44 = 0;
    self->field_48 = 0;
    return self;
}
