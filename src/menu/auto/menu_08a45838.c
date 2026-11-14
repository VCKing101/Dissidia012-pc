#include "menu/auto_region2.h"

/* Screen47D90: releases its four texts. */
void func_08A45838(Screen47D90 *self)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}
