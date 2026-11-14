#include "menu/auto_region2.h"

/* Widget52C98: releases its five handles. */
void func_08A529F8(Widget52C98 *self)
{
    int i;

    for (i = 0; i < 5; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
