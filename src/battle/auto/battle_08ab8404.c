#include "battle/auto_region4.h"

/* PartB8390 reset: clears the header and the four ten-word tables. */
void func_08AB8404(PartB8390 *self)
{
    int i;

    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    for (i = 0; i < 10; i++) {
        self->field_0C[i] = 0;
        self->field_34[i] = 0;
        self->field_5C[i] = 0;
        self->field_84[i] = 0;
    }
}
