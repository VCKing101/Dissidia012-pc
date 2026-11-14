#include "battle/auto_region4.h"

/* Part86C80: field_24 while active, otherwise 0. */
unsigned char func_08A86C60(Part86C80 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_24;
    }
}
