#include "battle/auto_region4.h"

/* Part88B28: field_14 while active, otherwise 0. */
unsigned char func_08A87AD8(Part88B28 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_14;
    }
}
