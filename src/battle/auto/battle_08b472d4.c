#include "battle/auto_region5.h"

/* Part47170: releases both resources. */
void func_08B472D4(Part47170 *self)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
