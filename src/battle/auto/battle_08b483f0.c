#include "battle/auto_region5.h"

/* Part4785C: releases the two models. */
void func_08B483F0(Part4785C *self)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (self->models[i] != 0) {
            func_0887BA5C(self->models[i]);
            self->models[i] = 0;
        }
    }
}
