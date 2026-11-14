#include "battle/auto_region5.h"

/* Part1AAF8: whether field_04 is 1 while active, else 0. */
unsigned char func_08B1AE84(Part1AAF8 *self)
{
    if (self->active == 0) {
        return 0;
    } else {
        return self->field_04 == 1;
    }
}
