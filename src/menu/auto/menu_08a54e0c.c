#include "menu/auto_region2.h"

/* Widget54E98: releases its text. */
void func_08A54E0C(Widget54E98 *self)
{
    int i;

    for (i = 0; i < 1; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
