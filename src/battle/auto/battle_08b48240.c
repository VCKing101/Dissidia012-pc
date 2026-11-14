#include "battle/auto_region5.h"

/* Part4785C: releases the three resources. */
void func_08B48240(Part4785C *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (self->resources[i] != 0) {
            func_088DAB44(self->resources[i]);
            self->resources[i] = 0;
        }
    }
}
