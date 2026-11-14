#include "battle/auto_region4.h"

/* PartA1BD8 release: releases the Part7E948 at field_148, then resets. */
void func_08A9ADA8(void *object)
{
    PartA1BD8 *self = object;

    func_08A7E7E0(&self->field_148);
    func_08A9ACBC(self);
}
