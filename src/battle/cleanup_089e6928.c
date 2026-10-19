#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern unsigned char D_089E6E50[];
extern void func_08992F04(void *);
extern void func_089E9E68(void *, int);
extern void func_089E1D68(void *, int);

void func_089E6928(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt11DC *)object)->member_table = D_089E6E50;
        func_089E69C0(object);
        func_089E9E68(object + 0x124, 2);
        func_089E1D68(object + 8, 2);
        if (flags & 1) func_08992F04(object);
    }
}
