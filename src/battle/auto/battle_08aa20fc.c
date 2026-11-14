#include "battle/auto_region4.h"

/* PartA29D8: field_10 while active, otherwise 0. */
unsigned char func_08AA20FC(PartA29D8 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_10;
    }
}
