#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089E7390[];
extern unsigned char D_089E73A0[];
extern void func_08993560(void *, unsigned int, unsigned int, void *);

void *func_089E7130(unsigned char *object)
{
    ((BattleTableAt44 *)object)->member_table = D_089E7390;
    func_08993560(object + 4, 2, 0x20, D_089E73A0);
    func_089E71F8(object);
    return object;
}
