#include "battle/auto_region4.h"

/* Part9ABE8 release: releases the Part7E948 at field_0C, then resets. */
void func_08A9A5EC(void *object)
{
    Part9ABE8 *self = object;

    func_08A7E7E0(&self->field_0C);
    func_08A9A5DC(self);
}
