#include "menu/auto_region2.h"

/* Page67EE0: releases its four texts. */
void func_08A64100(Page67EE0 *self)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
