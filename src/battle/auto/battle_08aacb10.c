#include "battle/auto_region4.h"

/* PartADF68: field_18C while active, otherwise 0. */
int func_08AACB10(PartADF68 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return self->field_18C;
    }
}
