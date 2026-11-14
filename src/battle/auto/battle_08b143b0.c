#include "battle/auto_region5.h"

/* Part143B0 constructor: builds the block and clears the rest. */
Part143B0 *func_08B143B0(Part143B0 *self)
{
    func_0887A61C(self->field_4C);
    self->field_5C = 0;
    self->field_60 = 0;
    self->field_61 = 0;
    func_08935828(self->field_00, 0, sizeof(self->field_00));
    func_08935828(self->field_0C, 0, sizeof(self->field_0C));
    func_08935828(self->field_34, 0, sizeof(self->field_34));
    return self;
}
