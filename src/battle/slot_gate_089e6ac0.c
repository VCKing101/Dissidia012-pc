#include "battle/agent1_types.h"

/* Slot gate requests and per-slot switches. */
extern int func_089EA168(void *, void *);
extern int func_089E6DA0(BattleSlotGate *);

int func_089E6AC0(BattleSlotGate *gate, void *request)
{
    if (!gate->field_00) return -1;
    if (!request) return -2;
    if (!gate->field_05) {
        if (func_089EA168(gate->field_124, request)) return -3;
    }
    return 0;
}
