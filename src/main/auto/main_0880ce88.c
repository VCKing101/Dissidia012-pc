#include "main/auto_region1.h"

/* (a^2 - b^2 + c^2) / 2c: how far along side c the foot of the height from
 * the opposite corner lies, for a triangle with sides a, b, c. */
float func_0880CE88(float a, float b, float c)
{
    return ((a * a - b * b) + c * c) / (c * 2.0f);
}
