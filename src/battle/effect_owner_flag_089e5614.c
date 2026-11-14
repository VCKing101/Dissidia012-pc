#include "battle/agent1_types.h"

int func_089E5614(BattleEffectOwner *owner)
{
    if (!owner->field_00) return -1;
    owner->field_D0->field_20 |= 0x2000;
    return 0;
}
