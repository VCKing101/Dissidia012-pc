#include "battle/auto_region3.h"

typedef struct Phase31334 {
    char pad_00[8];
    int field_08;
} Phase31334;

/* 0.85f minus the factor func_08A3EB2C reports while field_08 is 4,
 * otherwise 0. */
float func_08A31334(Phase31334 *self)
{
    float value;

    if (self->field_08 == 4) {
        value = func_08A3EB2C(D_08A405E0);
        value = 0.85f - value;
    } else {
        value = 0.0f;
    }
    return value;
}
