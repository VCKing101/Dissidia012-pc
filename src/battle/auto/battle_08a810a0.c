#include "battle/auto_region4.h"

/* Part815C8: field_10 while active, otherwise 0. */
unsigned char func_08A810A0(Part815C8 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_10;
    }
}
