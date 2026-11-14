#include "main/auto_region4.h"

/* Obj8FBBE0: releases all its texts. */
void func_088FC588(Obj8FBBE0 *self)
{
    int i;

    for (i = 0; i < 0x1d; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}
