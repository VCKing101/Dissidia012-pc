#include "menu/auto_region2.h"

/* MenuRoot2: releases texts 8..0x13. */
void func_08A41304(MenuRoot2 *self)
{
    int i;

    for (i = 8; i < 0x14; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}
