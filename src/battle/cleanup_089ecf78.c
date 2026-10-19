#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern unsigned char D_089ED850[];
extern void func_08992F04(void *);

void func_089ECF78(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt20 *)object)->member_table = D_089ED850;
        func_089ED01C(object);
        if (flags & 1) func_08992F04(object);
    }
}
