#include "menu/auto_region2.h"

/* Part5B5C0: releases its seven handles. */
void func_08A59680(Part5B5C0 *self)
{
    int i;

    for (i = 0; i < 7; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
