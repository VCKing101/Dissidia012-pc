#include "battle/agent1_types.h"

extern void func_089E4594(BattleTriggerSet *);

int func_089E45C0(BattleTriggerSet *set, unsigned int kind, int a, int b, int c, int d)
{
    func_089E4594(set);
    if (kind >= 2 || !a || !b || !c || !d) return -1;
    set->field_1C = kind;
    set->field_20 = a;
    set->field_24 = b;
    set->field_28 = c;
    set->field_2C = d;
    set->field_00 = 1;
    return 0;
}
