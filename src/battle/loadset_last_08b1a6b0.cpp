#include "battle/agent27_types.h"

extern "C" {

int func_08B1A6B0(LoadSet *set)
{
    if (set->active == 0) {
        return -1;
    }
    int best = -1;
    for (int i = 0; i < set->count; i++) {
        LoadRecord *record = &set->records[i];
        if (record->value <= best) {
            continue;
        }
        best = record->value;
    }
    return best;
}

}
