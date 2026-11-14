#include "battle/auto_region4.h"

/* PartAAD68 release: releases both PartA76A0, then resets. */
void func_08AA7850(PartAAD68 *self)
{
    func_08AA6B84(&self->field_DC);
    func_08AA6B84(&self->field_08);
    func_08AA7840(self);
}
