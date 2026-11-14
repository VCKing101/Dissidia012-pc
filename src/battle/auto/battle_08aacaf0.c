#include "battle/auto_region4.h"

/* PartADF68: the Inline2 at field_168 while active, otherwise null. */
Inline2 *func_08AACAF0(PartADF68 *self)
{
    if (!self->active) {
        return 0;
    } else {
        return &self->field_168;
    }
}
