#include "battle/agent1_types.h"

/* Constructor, destructor and reset of the 0x34-byte trigger set. */
extern unsigned char D_089E4B28[];
extern void func_089E4594(BattleTriggerSet *);
extern void func_08992F04(void *);
void func_089E4550(BattleTriggerSet *set);

BattleTriggerSet *func_089E44C0(BattleTriggerSet *set)
{
    set->vtable = D_089E4B28;
    func_089E4550(set);
    return set;
}

void func_089E44F8(BattleTriggerSet *set, int flags)
{
    if (set) {
        set->vtable = D_089E4B28;
        func_089E4594(set);
        if (flags & 1) func_08992F04(set);
    }
}

void func_089E4550(BattleTriggerSet *set)
{
    int i;

    set->field_00 = 0;
    set->field_04 = 0;
    set->field_08 = 0;
    for (i = 0; i < 4; i++) {
        set->field_0C[i] = 0;
    }
    set->field_1C = -1;
    set->field_20 = 0;
    set->field_24 = 0;
    set->field_28 = 0;
    set->field_2C = 0;
}
