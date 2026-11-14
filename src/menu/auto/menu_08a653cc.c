#include "menu/auto_region2.h"

/* Page68030: releases its two texts. */
void func_08A653CC(Page68030 *self)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
