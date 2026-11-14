#include "battle/auto_region3.h"

typedef struct Phase44354 {
    char pad_00[8];
    int field_08;
} Phase44354;

/* 1.3f minus the factor func_08A3EB2C reports while field_08 is 1, otherwise
 * 0. */
float func_08A44354(Phase44354 *self)
{
    float value;

    if (self->field_08 == 1) {
        value = func_08A3EB2C(D_08A405E0);
        value = 1.3f - value;
    } else {
        value = 0.0f;
    }
    return value;
}
