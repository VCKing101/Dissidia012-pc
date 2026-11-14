extern "C" {
#include "battle/agent1_types.h"

extern unsigned char func_089E4A70(void *);
}

struct PairEntryOps {
    static int any_idle(BattlePairEntry *entry)
    {
        int result = 0;

        if (!func_089E4A70(&entry->field_08) || !func_089E4A70(&entry->field_3C)) result = 1;
        return result;
    }
};

extern "C" unsigned char func_089E3C70(BattlePairEntry *entry)
{
    if (!entry->field_00) return 0;
    return PairEntryOps::any_idle(entry);
}
