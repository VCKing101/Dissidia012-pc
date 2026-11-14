#include "battle/auto_region3.h"

/* field_14 while active, otherwise 0. */
unsigned char func_08A7B448(Part7E728 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_14;
    }
}
