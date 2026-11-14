#include "battle/auto_region5.h"

/* Part4785C: releases the four handles. */
void func_08B48300(Part4785C *self)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
