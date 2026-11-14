#include "menu/auto_region2.h"

/* Widget54438: releases its thirteen handles. */
void func_08A53B1C(Widget54438 *self)
{
    int i;

    for (i = 0; i < 13; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
