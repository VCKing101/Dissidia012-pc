#include "battle/auto_region5.h"

/* Part28570: releases the block at field_14 if it is in use. */
void func_08B27AE0(Part28570 *self)
{
    if (func_0887AFB0(self->field_14) == 1) {
        func_0887AD1C(self->field_14);
    }
}
