#include "menu/auto_region2.h"

/* Cell5C5C8: releases its two texts. */
void func_08A5CA8C(Cell5C5C8 *self)
{
    int i;

    for (i = 0; i < 2; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
