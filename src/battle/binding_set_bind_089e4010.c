#include "battle/agent1_types.h"

extern void func_089E3FE4(BattleBindingSet *);

int func_089E4010(BattleBindingSet *set, void *resource)
{
    func_089E3FE4(set);
    if (!resource) return -1;
    set->field_04 = resource;
    set->field_00 = 1;
    return 0;
}
