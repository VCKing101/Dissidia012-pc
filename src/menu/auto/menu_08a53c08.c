#include "menu/auto_region2.h"

/* Widget54438: releases its three texts. */
void func_08A53C08(Widget54438 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
