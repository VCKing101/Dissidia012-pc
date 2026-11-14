#include "battle/agent1_types.h"

/* Slot gate: re-evaluates the any-slot flag and toggles the motion set. */
extern void func_089E2164(BattleMotionSet *, int);
extern void func_089E20FC(BattleMotionSet *, int);
extern void func_089EA2C0(void *);
extern void func_089EA324(void *);
int func_089E6DA0(BattleSlotGate *gate)
{
    unsigned char any = 0;
    {
        int i;
        for (i = 0; i < 4; i++) {
            if (gate->field_01[i] == 1) {
                any = 1;
                break;
            }
        }
    }
    if (gate->field_05 != any) {
        if (!any) {
            func_089E20FC(&gate->field_08, 0);
            func_089EA2C0(gate->field_124);
        } else {
            func_089E2164(&gate->field_08, 0);
            func_089EA324(gate->field_124);
        }
        gate->field_05 = any;
    }
    return 0;
}
