#include "battle/agent1_types.h"

extern void func_089E5AA8(BattleEffectBind *);

void func_089E5AD0(BattleEffectBind *bind)
{
    func_089E5AA8(bind);
}

int func_089E5AEC(BattleEffectBind *bind, unsigned int kind, int value)
{
    func_089E5AD0(bind);
    if (kind >= 2 || !value) return -1;
    bind->field_0C = kind;
    bind->field_10 = value;
    bind->field_04 = 1;
    bind->field_00 = 1;
    return 0;
}
