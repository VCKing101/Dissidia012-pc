#include "menu/auto_region2.h"

/* Page67F88: releases its three handles. */
void func_08A64990(Page67F88 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
