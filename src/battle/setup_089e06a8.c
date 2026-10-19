#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089E0CA8[];
extern unsigned char D_089E11B8[];
extern void func_0887A61C(void *);
extern void func_08993560(void *, unsigned int, unsigned int, void *);

void *func_089E06A8(unsigned char *object)
{
    ((BattleTableAt94 *)object)->member_table = D_089E0CA8;
    func_0887A61C(object + 8);
    func_08993560(object + 0x18, 4, 0x1C, D_089E11B8);
    func_089E0CB8(object + 0x88);
    func_089E07A0(object);
    return object;
}
