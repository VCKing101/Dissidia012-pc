#include "main/auto_region4.h"

/* Clamps value to the range of a short. */
int func_088FF78C(int value)
{
    if (0x7fff < value) {
        value = 0x7fff;
    }
    if (value < -0x8000) {
        value = -0x8000;
    }
    return value;
}
