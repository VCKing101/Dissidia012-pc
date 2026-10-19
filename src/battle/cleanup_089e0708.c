#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern unsigned char D_089E0CA8[];
extern unsigned char D_089E11F0[];
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_0887A644(void *, int);
extern void func_089E0C6C(void *);
extern void func_089E0CF0(void *, int);

void func_089E0708(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt94 *)object)->member_table = D_089E0CA8;
        func_089E07AC(object);
        func_089E0C6C(object);
        func_089E0CF0(object + 0x88, 2);
        func_0899372C(object + 0x18, 4, 0x1C, D_089E11F0, 0, 0);
        func_0887A644(object + 8, 2);
        if (flags & 1) func_08992F04(object);
    }
}
