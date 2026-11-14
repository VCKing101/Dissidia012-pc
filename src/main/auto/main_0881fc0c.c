#include "main/auto_region2.h"

/* Adds field_3C to the entry of values that *index selects. */
void func_0881FC0C(Obj81FC0C *self, float *values)
{
    float *value;

    value = &values[*self->index];
    *value = *value + self->field_3C;
}
