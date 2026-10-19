#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern unsigned char D_089E70D8[];
extern void func_08992F04(void *);

void func_089E6E98(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt8 *)object)->member_table = D_089E70D8;
        func_089E6EFC(object);
        if (flags & 1) func_08992F04(object);
    }
}
