#include "battle/auto_region5.h"

/* Part1C008: whether field_08 is 4 while active, else 0. */
unsigned char func_08B1C9C0(Part1C008 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_08 == 4;
    }
}
