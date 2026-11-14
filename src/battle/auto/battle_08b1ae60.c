#include "battle/auto_region5.h"

/* Part1AAF8: whether field_0C is below 0x21 while active, else 0. */
unsigned char func_08B1AE60(Part1AAF8 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_0C < 0x21;
    }
}
