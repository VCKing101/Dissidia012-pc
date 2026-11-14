#include "battle/auto_region5.h"

/* Returns D_089D6A00's field_04 + 1 when its field_18 is set, else 1. */
int func_08B08BEC(void)
{
    int count;

    count = 1;
    if (D_089D6A00->field_18 == 1) {
        count = D_089D6A00->field_04 + 1;
    }
    return count;
}
