#include "menu/auto_region2.h"

/* Cell5C5C8: releases its four handles. */
void func_08A5C98C(Cell5C5C8 *self)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
