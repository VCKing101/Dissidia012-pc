#include "battle/auto_region3.h"

/* Address of entry `index` of the table D_08A50568 points to. */
Table50568Entry *func_08A50174(int index)
{
    return &D_08A50568->entries[index];
}
