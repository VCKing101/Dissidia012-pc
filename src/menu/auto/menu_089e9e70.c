#include "menu/auto_region1.h"

/* Block9E70 constructor: clears field_14 and both 8-byte fields. */
Block9E70 *func_089E9E70(Block9E70 *self)
{
    self->field_14 = 0;
    func_08935828(self->field_00, 0, sizeof(self->field_00));
    func_08935828(self->field_08, 0, sizeof(self->field_08));
    return self;
}
