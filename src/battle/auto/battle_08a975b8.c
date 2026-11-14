#include "battle/auto_region4.h"

/* Part98058: field_1C as a float while active, otherwise 0. */
float func_08A975B8(Part98058 *self)
{
    if (!self->active) {
        return 0.0f;
    } else {
        return (float)self->field_1C;
    }
}
