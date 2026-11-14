#include "battle/auto_region4.h"

/* PartA4250 release: releases the Part7E948 at field_08, then resets. */
void func_08AA2AB8(void *object)
{
    PartA4250 *self = object;

    func_08A7E7E0(&self->field_08);
    func_08AA2A94(self);
}
