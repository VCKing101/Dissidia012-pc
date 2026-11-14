#include "battle/agent1_types.h"

/* Constructor, destructor, reset and release of the pool's model set. */
extern unsigned char D_089DF4B8[];
extern void func_088DA060(void *);
extern void func_08992F04(void *);
void func_089DF148(BattleModelSet *set);
void func_089DF170(BattleModelSet *set);

BattleModelSet *func_089DF0B8(BattleModelSet *set)
{
    set->vtable = D_089DF4B8;
    func_089DF148(set);
    return set;
}

void func_089DF0F0(BattleModelSet *set, int flags)
{
    if (set) {
        set->vtable = D_089DF4B8;
        func_089DF170(set);
        if (flags & 1) func_08992F04(set);
    }
}

void func_089DF148(BattleModelSet *set)
{
    int i;

    set->field_00 = 0;
    set->field_04 = 0;
    for (i = 0; i < 3; i++) {
        set->field_08[i] = 0;
    }
}

void func_089DF170(BattleModelSet *set)
{
    {
        int i;
        for (i = 0; i < 3; i++) {
            if (set->field_08[i]) {
                func_088DA060(set->field_08[i]);
                set->field_08[i] = 0;
            }
        }
    }
    if (set->field_04) {
        func_088DA060(set->field_04);
        set->field_04 = 0;
    }
    func_089DF148(set);
}
