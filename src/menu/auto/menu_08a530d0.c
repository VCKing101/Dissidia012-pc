#include "menu/auto_region2.h"

/* Elem52D60: releases its seven handles. */
void func_08A530D0(Elem52D60 *self)
{
    int i;

    for (i = 0; i < 7; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}
