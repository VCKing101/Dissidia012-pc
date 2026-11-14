#include "battle/auto_region5.h"

/* Part49220 reset. */
void func_08B49248(Part49220 *self)
{
    int i;

    self->phase = 0;
    for (i = 0; i < 7; i++) {
        self->handles[i] = 0;
    }
    for (i = 0; i < 1; i++) {
        self->models[i] = 0;
    }
    self->field_24 = 0;
    self->field_28 = 0;
}
