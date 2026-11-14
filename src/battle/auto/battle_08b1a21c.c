#include "battle/auto_region5.h"

/* Part1A0D8: whether field_08 is 0 while active, else 0. */
unsigned char func_08B1A21C(Part1A0D8 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_08 == 0;
    }
}
