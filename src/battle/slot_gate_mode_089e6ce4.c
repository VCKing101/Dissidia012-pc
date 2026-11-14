#include "battle/agent1_types.h"

/* Slot gate: pick the motion/request mode from the global battle state. */
typedef struct BattleGlobalState { unsigned char padding_000[0x278]; int field_278; } BattleGlobalState;
extern BattleGlobalState *D_08A405E0;
extern void func_089E2164(BattleMotionSet *, int);
extern void func_089E20FC(BattleMotionSet *, int);
extern void func_089EA0AC(void *, int);
extern int func_08A50140(void);
int func_089E6CE4(BattleSlotGate *gate)
{
    if ((unsigned char)(D_08A405E0->field_278 ? 1 : 0) == 1) {
        func_089E2164(&gate->field_08, 1);
        func_089EA0AC(gate->field_124, 1);
    } else if (func_08A50140() == 1) {
        func_089E2164(&gate->field_08, 1);
        func_089EA0AC(gate->field_124, 0);
    } else {
        func_089E20FC(&gate->field_08, 1);
        func_089EA0AC(gate->field_124, 0);
    }
    return 0;
}
