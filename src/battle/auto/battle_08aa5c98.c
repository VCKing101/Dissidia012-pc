#include "battle/auto_region4.h"

/* PartA5830: field_02 while active, otherwise 0. */
unsigned char func_08AA5C98(PartA5830 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_02;
    }
}
