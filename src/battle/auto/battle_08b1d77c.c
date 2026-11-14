#include "battle/auto_region5.h"

/* Part1D810: releases field_44 and the three blocks that are in use. */
void func_08B1D77C(Part1D810 *self)
{
    func_08B19178(&self->field_44);
    if (func_0887AFB0(self->field_34) == 1) {
        func_0887AD1C(self->field_34);
    }
    if (func_0887AFB0(self->field_24) == 1) {
        func_0887AD1C(self->field_24);
    }
    if (func_0887AFB0(self->field_14) == 1) {
        func_0887AD1C(self->field_14);
    }
}
