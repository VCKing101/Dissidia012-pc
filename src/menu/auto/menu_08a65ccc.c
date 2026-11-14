#include "menu/auto_region2.h"

/* Page680D8: releases its first seven handles. */
void func_08A65CCC(Page680D8 *self)
{
    int i;

    for (i = 0; i < 7; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
