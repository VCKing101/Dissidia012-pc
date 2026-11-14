#include "battle/agent1_types.h"

/* Pair owner queries. */
extern int func_089E3C3C(BattlePairEntry *);
extern int func_089E3C70(BattlePairEntry *);

int func_089E3674(BattlePairOwner *owner, unsigned int index)
{
    if (!owner->field_00) return -1;
    if (index >= 2) return -2;
    if (func_089E3C3C(&owner->field_08[index])) return -3;
    return 0;
}

int func_089E36E8(BattlePairOwner *owner)
{
    int result;

    if (!owner->field_00) return 0;
    result = 0;
    {
        int i;
        for (i = 0; i < 2; i++) {
            if (!func_089E3C70(&owner->field_08[i])) continue;
            result = 1;
            goto done;
        }
    }
done:
    return result;
}
