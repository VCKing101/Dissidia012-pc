#include "menu/auto_region2.h"

/* Part5B578: appends value to values[]. */
void func_08A58C24(Part5B578 *self, int value)
{
    int count;

    count = self->count;
    self->values[count] = value;
    self->count = count + 1;
}
