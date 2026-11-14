#include "battle/agent1_types.h"

/* Motion set per-frame step. */
extern int func_089E8EF0(int);
extern int func_089E256C(BattleMotionTrack *);
extern void func_089E2630(BattleMotionTrack *);
extern void func_089E2CC4(BattleMotionLink *);

int func_089E2044(BattleMotionSet *set)
{
    int mode;

    if (!set->field_00) return -1;
    mode = func_089E8EF0(set->field_04);
    if (!set->field_117) {
        if (mode != 2) {
            int i;
            for (i = 0; i < 2; i++) {
                if (func_089E256C(&set->field_44[i]) == 1) {
                    func_089E2630(&set->field_44[i]);
                    func_089E2CC4(&set->field_0C[i]);
                }
            }
        }
    }
    return 0;
}
