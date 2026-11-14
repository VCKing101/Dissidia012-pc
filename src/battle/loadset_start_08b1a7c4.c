#include "battle/agent27_types.h"

int func_08B1A7C4(LoadSet *set)
{
    if (set->active == 0) {
        return -1;
    }
    set->field_08 = 0;
    set->count = 0;
    set->state = 1;
    return 0;
}
