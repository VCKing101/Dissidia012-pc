#include "battle/agent27_types.h"

int func_08B1CA0C(StreamSet *set)
{
    if (set->active == 0) {
        return -1;
    }
    set->field_10 = 0;
    set->count = 0;
    set->phase = 0;
    return 0;
}
