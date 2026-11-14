#include "menu/auto_region2.h"

/* Widget52C98: releases its two texts. */
void func_08A52C08(Widget52C98 *self)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
