#include "battle/auto_region5.h"

/* Part44858 constructor: builds the Part44B78 base. */
Part44858 *func_08B44510(Part44858 *self)
{
    func_08B44B78(&self->base);
    self->base.vtable = D_08B44858;
    return self;
}
