#include "battle/auto_region4.h"

/* Part97108 release: releases the PartB3A30 at field_0C, then resets. */
void func_08A9649C(void *object)
{
    Part97108 *self = object;

    func_08AB37FC(&self->field_0C);
    func_08A96444(self);
}
