#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern unsigned char D_089E38C0[];
extern unsigned char D_089E3920[];
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_089E3300(void *);

void func_089E3258(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt1C0 *)object)->member_table = D_089E38C0;
        func_089E3300(object);
        func_0899372C(object + 8, 2, 0xD8, D_089E3920, 0, 0);
        if (flags & 1) func_08992F04(object);
    }
}
