#include "battle/auto_region5.h"

/* Part16D98 constructor: clears it. */
Part16D98 *func_08B16D98(Part16D98 *self)
{
    self->phase = 0;
    self->step = 0;
    self->field_30 = 0;
    func_08935828(self->sources, 0, sizeof(self->sources));
    func_08935828(self->handles, 0, sizeof(self->handles));
    func_08935828(self->field_20, 0, sizeof(self->field_20));
    return self;
}
