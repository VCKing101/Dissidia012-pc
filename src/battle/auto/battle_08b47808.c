#include "battle/auto_region5.h"

/* Part47358: releases the five models. */
void func_08B47808(Part47358 *self)
{
    int i;

    for (i = 0; i < 5; i++) {
        if (self->models[i] != 0) {
            func_0887BA5C(self->models[i]);
            self->models[i] = 0;
        }
    }
}
