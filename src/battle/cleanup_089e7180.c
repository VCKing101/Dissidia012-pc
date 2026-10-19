#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern unsigned char D_089E7390[];
extern unsigned char D_089E73D8[];
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_089E7200(void *);

void func_089E7180(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt44 *)object)->member_table = D_089E7390;
        func_089E7200(object);
        func_0899372C(object + 4, 2, 0x20, D_089E73D8, 0, 0);
        if (flags & 1) func_08992F04(object);
    }
}
