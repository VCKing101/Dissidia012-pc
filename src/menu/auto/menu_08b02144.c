#include "menu/auto_region4.h"

/* Screen02144: releases its first text and resets. */
int func_08B02144(Screen02144 *self)
{
    int i;

    for (i = 0; i < 1; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    func_08B02268(self);
    self->work->field_63C = self->work->field_640;
    return 0;
}
