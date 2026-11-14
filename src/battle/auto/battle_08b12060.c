#include "battle/auto_region5.h"

/* Part12060 constructor: clears it. */
Part12060 *func_08B12060(Part12060 *self)
{
    self->field_00 = 0;
    self->field_18 = 0;
    func_08935828(self->field_04, 0, sizeof(self->field_04));
    return self;
}
