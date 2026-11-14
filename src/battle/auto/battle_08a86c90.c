#include "battle/auto_region4.h"

/* Part870D8 constructor: builds the three Part86C80 at field_20. */
Part870D8 *func_08A86C90(Part870D8 *self)
{
    self->vtable = D_08A870D8;
    func_08993560(self->field_20, 3, sizeof(Part86C80), func_08A86958);
    func_08A86D58(self);
    return self;
}
