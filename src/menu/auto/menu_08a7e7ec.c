#include "menu/auto_region3.h"

/* Step7E7EC: on input bit 2, plays the leave motions; state 1. */
void func_08A7E7EC(Step7E7EC *self, unsigned int flags)
{
    if ((flags & 2) != 0) {
        func_08A743E0(&self->field_04);
        func_08A72F5C(&self->field_14);
        self->state = 1;
    }
}
