#include "battle/auto_region4.h"

/* PartB32A8 release: releases every member, then resets. */
void func_08AB3014(void *object)
{
    PartB32A8 *self = object;

    func_08A86D94(&self->field_440);
    func_08A7EA74(&self->field_390);
    func_08A7B288(&self->field_36C);
    func_08A64F44(&self->field_360);
    func_08AAE22C(&self->field_34C);
    func_08A824BC(&self->field_198);
    func_08A97240(&self->field_04);
    func_08AB300C(self);
}
