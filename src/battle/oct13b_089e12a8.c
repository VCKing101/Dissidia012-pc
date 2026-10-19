#include "battle/oct13_activation_views.h"

int func_089E12A8(void *object, unsigned int index, void *resource, void *shared)
{
    func_089E1268(object);
    if (index >= 4 || !resource || !shared) return -1;
    ((BattleOct13Bind14 *)object)->field_0C = index;
    ((BattleOct13Bind14 *)object)->field_10 = resource;
    ((BattleOct13Bind14 *)object)->field_14 = shared;
    ((BattleOct13Bind14 *)object)->field_00 = 1;
    return 0;
}
