#include "menu/auto_region2.h"

/* Part5B5C0: releases its eight texts. */
void func_08A597B0(Part5B5C0 *self)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
