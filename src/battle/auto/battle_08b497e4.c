#include "battle/auto_region5.h"

/* Part49220: releases the model. */
void func_08B497E4(Part49220 *self)
{
    int i;

    for (i = 0; i < 1; i++) {
        if (self->models[i] != 0) {
            func_0887BA5C(self->models[i]);
            self->models[i] = 0;
        }
    }
}
