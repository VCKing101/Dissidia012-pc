#include "battle/auto_region5.h"

/* PartF6A34 constructor: clears it. */
PartF6A34 *func_08AF6A34(PartF6A34 *self)
{
    self->field_15C = 0;
    func_08AF3B90(&self->field_160);
    self->phase = 0;
    self->field_170 = 0;
    func_08935828(self->field_00, 0, sizeof(self->field_00));
    func_08935828(self->handles, 0, sizeof(self->handles));
    func_08935828(self->field_134, 0, sizeof(self->field_134));
    return self;
}
