#include "battle/agent1_types.h"

/* Motion track entry lookups. */
BattleMotionEntry func_089E25E4(BattleMotionTrack *track, unsigned int index);

int func_089E256C(BattleMotionTrack *track)
{
    BattleMotionEntry entry;
    int result;

    if (!track->field_00) return 0;
    {
        int i;
        for (i = 0; i < 10; i++) {
            entry = func_089E25E4(track, i);
            if ((unsigned int)entry.id < 11) {
                result = 1;
                goto done;
            }
        }
    }
    result = 0;
done:
    return result;
}

BattleMotionEntry func_089E25E4(BattleMotionTrack *track, unsigned int index)
{
    BattleMotionEntry *entries;

    if (index >= 10) {
        BattleMotionEntry none;
        none.id = -1;
        none.value = 0.0f;
        return none;
    }
    entries = track->entries;
    return entries[index];
}
