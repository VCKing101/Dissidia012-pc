#include "main/auto_region2.h"

/* Stores 1 / value in field_3B4 (1 when value is 0). */
void func_08834D60(ActorView *self, float value)
{
    if (value == 0.0f) {
        value = 1.0f;
    } else {
        value = 1.0f / value;
    }
    self->field_3B4 = value;
}
