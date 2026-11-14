#include "menu/auto_region2.h"

/* Item: releases its six handles. */
void func_08A5550C(Item *self)
{
    int i;

    for (i = 0; i < 6; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
