#include "battle/auto_region4.h"

/* Part86940: field_0C while active, otherwise 0. */
unsigned char func_08A85EBC(Part86940 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_0C;
    }
}
