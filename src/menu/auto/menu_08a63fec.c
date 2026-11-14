#include "menu/auto_region2.h"

/* Page67EE0: releases its first five handles. */
void func_08A63FEC(Page67EE0 *self)
{
    int i;

    for (i = 0; i < 5; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
