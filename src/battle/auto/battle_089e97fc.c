#include "battle/auto_region1.h"

/* field_18 of the looked-up entry's stats, scaled by 100 and divided by
 * 10000 in integers; 0 when there is no entry. */
float func_089E97FC(unsigned int index)
{
    LookupEntry *entry = func_089E95B0(index);

    if (entry == 0) {
        return 0.0f;
    } else {
        return (float)((int)(entry->field_52C->field_18 * 100.0f) / 10000);
    }
}
