#include "battle/auto_region3.h"

/* True when the first word of the table D_08A50568 points to is set. */
unsigned char func_08A50140(void)
{
    return D_08A50568->field_00 != 0;
}
