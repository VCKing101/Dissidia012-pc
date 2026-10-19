#include "battle/oct13_activation_views.h"

int func_089E0D70(void *object, void *resource)
{
    func_089E0D54(object);
    if (!resource) return -1;
    ((BattleOct13Prefix04 *)object)->field_04 = resource;
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}
