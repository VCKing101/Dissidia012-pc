#include "battle/auto_region5.h"

/* Part47358: releases the three resources. */
void func_08B47710(Part47358 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
