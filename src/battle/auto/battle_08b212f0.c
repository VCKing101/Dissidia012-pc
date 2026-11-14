#include "battle/auto_region5.h"

/* Part22EF8 constructor. */
Part22EF8 *func_08B212F0(Part22EF8 *self)
{
    self->vtable = D_08B22EF8;
    func_08993560(self->field_14, 2, sizeof(Part05658), func_08A04928);
    func_08B213B8(self);
    return self;
}
