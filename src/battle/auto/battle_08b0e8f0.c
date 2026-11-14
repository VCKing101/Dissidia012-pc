#include "battle/auto_region5.h"

/* Part0E8F0 constructor: clears it. */
Part0E8F0 *func_08B0E8F0(Part0E8F0 *self)
{
    self->field_40 = 0;
    func_08935828(self->field_00, 0, sizeof(self->field_00));
    func_08935828(self->field_44, 0, sizeof(self->field_44));
    return self;
}
