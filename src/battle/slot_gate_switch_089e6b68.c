#include "battle/agent1_types.h"

extern int func_089E6DA0(BattleSlotGate *);

int func_089E6B68(BattleSlotGate *gate, unsigned int slot)
{
    if (!gate->field_00) return -1;
    if (slot >= 4) return -2;
    gate->field_01[slot] = 0;
    if (func_089E6DA0(gate)) return -3;
    return 0;
}

int func_089E6BD0(BattleSlotGate *gate, unsigned int slot)
{
    if (!gate->field_00) return -1;
    if (slot >= 4) return -2;
    gate->field_01[slot] = 1;
    if (func_089E6DA0(gate)) return -3;
    return 0;
}
