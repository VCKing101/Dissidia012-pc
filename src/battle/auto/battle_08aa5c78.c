#include "battle/auto_region4.h"

/* PartA5830: field_01 while active, otherwise 0. */
unsigned char func_08AA5C78(PartA5830 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_01;
    }
}
