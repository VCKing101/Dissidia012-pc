#include "main/auto_region2.h"

/* The float at field_24->field_568->field_2C, or 1 when it is 0. */
float func_08823234(Obj823234 *self)
{
    float value;

    value = self->field_24->field_568->field_2C;
    if (value == 0.0f) {
        value = 1.0f;
    }
    return value;
}
