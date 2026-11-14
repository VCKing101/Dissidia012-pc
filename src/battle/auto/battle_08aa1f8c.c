#include "battle/auto_region4.h"

/* PartA29D8 release: releases the Part7E948 at field_14, then resets. */
void func_08AA1F8C(void *object)
{
    PartA29D8 *self = object;

    func_08A7E7E0(&self->field_14);
    func_08AA1F74(self);
}
