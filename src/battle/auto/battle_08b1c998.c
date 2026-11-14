#include "battle/auto_region5.h"

/* Part1C008: whether field_08 is 1 while active, else 0. */
unsigned char func_08B1C998(Part1C008 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_08 == 1;
    }
}
