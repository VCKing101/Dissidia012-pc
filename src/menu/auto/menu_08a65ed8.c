#include "menu/auto_region2.h"

/* Page680D8: releases its three texts. */
void func_08A65ED8(Page680D8 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
