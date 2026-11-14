#include "menu/auto_region1.h"

/* Sum of the five counts in D_08A29BF0, 0 without it. */
int func_08A29660(void)
{
    int total;

    total = 0;
    if (D_08A29BF0 != 0) {
        total = (unsigned int)D_08A29BF0->field_02 + (unsigned int)D_08A29BF0->field_04 +
                (unsigned int)D_08A29BF0->field_06 + (unsigned int)D_08A29BF0->field_08 +
                (unsigned int)D_08A29BF0->field_0A;
    }
    return total;
}
