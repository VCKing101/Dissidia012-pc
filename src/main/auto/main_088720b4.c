#include "main/auto_region2.h"

/* Sets field_50 of both items of D_089D6008's first array. */
void func_088720B4(int value)
{
    int i;

    i = 0;
    do {
        D_089D6008->items60[i].field_50 = value;
        i = i + 1;
    } while (i < 2);
}
