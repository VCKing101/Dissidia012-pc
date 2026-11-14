#include "battle/agent1_types.h"

/* Pair owner per-entry forwarders. */
extern int func_089E3B60(BattlePairEntry *, int, unsigned char, unsigned char);
extern int func_089E3C3C(BattlePairEntry *);

int func_089E358C(BattlePairOwner *owner, unsigned int index, int value, unsigned char a, unsigned char b)
{
    if (!owner->field_00) return -1;
    if (index >= 2) return -2;
    if (func_089E3B60(&owner->field_08[index], value, a, b)) return -3;
    return 0;
}
