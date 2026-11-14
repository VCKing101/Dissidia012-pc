#include "battle/agent1_types.h"

/* Model set queries: per-slot transform update and lookups. */
extern void func_088DA0A8(void *, int, void *, void *);
extern void func_088D9B84(void *, void *);
extern void func_088D9AF4(void *, void *);
int func_089DF3EC(BattleModelSet *set, unsigned int index);

int func_089DF2F0(BattleModelSet *set)
{
    float matrix[16];
    int extra;

    if (!set->field_00) return -1;
    {
        int i;
        for (i = 0; i < 3; i++) {
            int bone = func_089DF3EC(set, i);
            func_088DA0A8(set->field_04, bone, matrix, &extra);
            func_088D9B84(set->field_08[i], matrix);
            func_088D9AF4(set->field_08[i], &extra);
        }
    }
    return 0;
}

BattleModelRef *func_089DF390(BattleModelSet *set)
{
    if (!set->field_00) return 0;
    return set->field_04;
}

void *func_089DF3B0(BattleModelSet *set, unsigned int index)
{
    if (!set->field_00) return 0;
    if (index >= 3) return 0;
    return set->field_08[index];
}
