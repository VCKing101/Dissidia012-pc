#include "menu/auto_region2.h"

/* Elem52D60: releases its four texts. */
void func_08A5318C(Elem52D60 *self)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
