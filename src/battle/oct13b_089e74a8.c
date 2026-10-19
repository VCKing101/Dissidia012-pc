#include "battle/oct13_activation_views.h"

int func_089E74A8(void *object, unsigned int index, void *resource, void *shared)
{
    func_089E7450((BattleRelease7450 *)object);
    if (index >= 2 || !resource || !shared) return -1;
    ((BattleOct13Bind18 *)object)->field_10 = index;
    ((BattleOct13Bind18 *)object)->field_14 = resource;
    ((BattleOct13Bind18 *)object)->field_18 = shared;
    ((BattleOct13Bind18 *)object)->field_00 = 1;
    return 0;
}
