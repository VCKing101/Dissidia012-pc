#include "battle/auto_region4.h"

/* Random float between `low` and `high`. */
float func_08AC3D20(float low, float high)
{
    float value;

    value = func_08AC3CD0();
    return (high - low) * value + low;
}
