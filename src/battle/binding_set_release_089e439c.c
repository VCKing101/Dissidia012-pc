#include "battle/agent1_types.h"

/* Releases every model held by the binding set. */
extern void func_088DA060(void *);

int func_089E439C(BattleBindingSet *set)
{
    {
        int i;
        for (i = 0; i < 4; i++) {
            int j;
            for (j = 0; j < 2; j++) {
                if (set->field_10[i][j]) {
                    func_088DA060(set->field_10[i][j]);
                    set->field_10[i][j] = 0;
                }
            }
        }
    }
    if (set->field_0C) {
        func_088DA060(set->field_0C);
        set->field_0C = 0;
    }
    return 0;
}
