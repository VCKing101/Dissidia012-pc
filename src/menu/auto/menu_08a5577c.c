#include "menu/auto_region2.h"

/* Item: releases its twelve texts. */
void func_08A5577C(Item *self)
{
    int i;

    for (i = 0; i < 12; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
