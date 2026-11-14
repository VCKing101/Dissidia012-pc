#include "battle/auto_region5.h"

/* Part05240: releases the block at field_0C if it is in use. */
void func_08B04D10(Part05240 *self)
{
    if (func_0887AFB0(self->field_0C) == 1) {
        func_0887AD1C(self->field_0C);
    }
}
