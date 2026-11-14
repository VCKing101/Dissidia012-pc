#include "battle/auto_region5.h"

/* Part47170 reset. */
void func_08B47198(Part47170 *self)
{
    int i;

    self->phase = 0;
    for (i = 0; i < 2; i++) {
        self->handles[i] = 0;
    }
}
