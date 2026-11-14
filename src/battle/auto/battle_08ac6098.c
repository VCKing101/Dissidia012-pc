#include "battle/auto_region4.h"

/* PartC6098 reset. */
void func_08AC6098(PartC6098 *self)
{
    int i;

    self->field_10 = 0;
    for (i = 0; i < 3; i++) {
        self->handles[i] = 0;
    }
}
