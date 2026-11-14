#include "main/auto_region4.h"

/* The rate of entry index (its value / 100) when it is the current entry and
 * enabled, else 1. (`current` must sort before `index`: SNC orders the
 * operands of the compare by name.) */
float func_088F0C18(int index)
{
    int current;
    float rate;

    current = func_088F1444();
    if ((current == index) && (current = func_088B6BE0(index), current != 0)) {
        current = func_088B6BA0(index);
        rate = (float)current / 100.0f;
    } else {
        rate = 1.0f;
    }
    return rate;
}
