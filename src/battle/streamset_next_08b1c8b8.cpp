#include "battle/agent27_types.h"

extern "C" {

int func_08B1C8B8(StreamSet *set, int after)
{
    if (set->active == 0) {
        return -1;
    }
    int best = 0x7FFFFFFF;
    for (int i = 0; i < set->count; i++) {
        LoadRecord *record = &set->records[i];
        if (record->value <= after || record->value >= best) {
            continue;
        }
        best = record->value;
    }
    if (best == 0x7FFFFFFF) {
        return -1;
    }
    return best;
}

}
