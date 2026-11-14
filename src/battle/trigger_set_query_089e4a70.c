#include "battle/agent1_types.h"

/* Trigger set: idle test and model release. */
extern void func_088DA060(void *);

int func_089E4A70(BattleTriggerSet *set)
{
    if (!set->field_00) return 0;
    return set->field_08 == 0;
}

void func_089E4A94(BattleTriggerSet *set)
{
    {
        int i;
        for (i = 0; i < 4; i++) {
            if (set->field_0C[i]) {
                func_088DA060(set->field_0C[i]);
                set->field_0C[i] = 0;
            }
        }
    }
    if (set->field_08) {
        func_088DA060(set->field_08);
        set->field_08 = 0;
    }
    if (set->field_04) {
        func_088DA060(set->field_04);
        set->field_04 = 0;
    }
}
