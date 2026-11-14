#include "battle/auto_region3.h"

/* field_02 while active, otherwise 0. */
unsigned char func_08A663E4(Part663E4 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_02;
    }
}
