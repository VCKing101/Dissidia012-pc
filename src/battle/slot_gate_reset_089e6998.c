#include "battle/agent1_types.h"

void func_089E6998(BattleSlotGate *gate)
{
    int i;

    gate->field_00 = 0;
    for (i = 0; i < 4; i++) {
        gate->field_01[i] = 0;
    }
    gate->field_05 = 0;
}
