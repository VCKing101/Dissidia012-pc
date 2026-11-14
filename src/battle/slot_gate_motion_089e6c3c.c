#include "battle/agent1_types.h"

/* Slot gate: switch the embedded motion set on or off. */
extern void func_089E2164(BattleMotionSet *, int);
extern void func_089E20FC(BattleMotionSet *, int);

int func_089E6C3C(BattleSlotGate *gate)
{
    if (!gate->field_00) return -1;
    func_089E2164(&gate->field_08, 2);
    return 0;
}

int func_089E6C74(BattleSlotGate *gate)
{
    if (!gate->field_00) return -1;
    func_089E20FC(&gate->field_08, 2);
    return 0;
}
