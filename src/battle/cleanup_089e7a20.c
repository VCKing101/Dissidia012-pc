#include "battle/runtime_helpers.h"

/* Observed lifecycle cleanup; original classes and ownership remain unknown. */
extern unsigned char D_089E8228[];
extern unsigned char D_089E5270[];
extern unsigned char D_089E8FA0[];
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_089DE0A0(void *, int);
extern void func_089E7AFC(void *);

void func_089E7A20(unsigned char *object, int flags)
{
    if (object) {
        ((BattleTableAt864 *)object)->member_table = D_089E8228;
        func_089E7AFC(object);
        func_089E6E98(object + 0x858, 2);
        func_0899372C(object + 0x808, 2, 0x28, D_089E8FA0, 0, 0);
        func_0899372C(object + 0x650, 2, 0xDC, D_089E5270, 0, 0);
        func_089DE0A0(object + 4, 2);
        if (flags & 1) func_08992F04(object);
    }
}
