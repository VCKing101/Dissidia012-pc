#include "battle/auto_region4.h"

/* PartADF68: the first word of field_280 while active, otherwise 0. */
int func_08AAD1C4(PartADF68 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_280.field_00;
    }
}
