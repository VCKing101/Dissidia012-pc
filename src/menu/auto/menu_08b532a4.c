#include "menu/auto_region4.h"

/* Widget52EB0: waits 0x5A frames, then moves to state 1. */
void func_08B532A4(Widget52EB0 *self)
{
    if (self->field_60 < 0x5A) {
        self->field_60 = self->field_60 + 1;
    }
    else {
        self->state = 1;
    }
}
