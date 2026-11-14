#include "battle/auto_region5.h"

/* Part4785C: sets up field_58 from the resources and starts it (phase
 * 3). */
void func_08B47E48(Part4785C *self)
{
    func_08B49290(&self->field_58, self->resources[2], self->handles[3], self->field_50,
                  self->field_54, 2);
    func_08B4934C(&self->field_58, self->field_4E8, 0);
    self->phase = 3;
}
