#include "battle/auto_region5.h"

/* Part49220: releases the first six resources. */
void func_08B49710(Part49220 *self)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
