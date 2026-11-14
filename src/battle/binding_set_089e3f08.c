#include "battle/agent1_types.h"

/* Constructor, destructor and reset of the 0x34-byte binding set. */
extern unsigned char D_089E4440[];
extern void func_089E3FE4(BattleBindingSet *);
extern void func_08992F04(void *);
void func_089E3F98(BattleBindingSet *set);

BattleBindingSet *func_089E3F08(BattleBindingSet *set)
{
    set->vtable = D_089E4440;
    func_089E3F98(set);
    return set;
}

void func_089E3F40(BattleBindingSet *set, int flags)
{
    if (set) {
        set->vtable = D_089E4440;
        func_089E3FE4(set);
        if (flags & 1) func_08992F04(set);
    }
}

void func_089E3F98(BattleBindingSet *set)
{
    int i;
    int j;

    set->field_00 = 0;
    set->field_04 = 0;
    set->field_08 = -1;
    set->field_0C = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            set->field_10[i][j] = 0;
        }
    }
}
