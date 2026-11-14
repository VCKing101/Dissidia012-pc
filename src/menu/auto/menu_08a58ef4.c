#include "menu/auto_region2.h"

/* Part5B578: releases its four handles. */
void func_08A58EF4(Part5B578 *self)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
