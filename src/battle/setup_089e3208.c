#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089E38C0[];
extern unsigned char D_089E38D0[];
extern void func_08993560(void *, unsigned int, unsigned int, void *);
extern void func_089E32D0(void *);

void *func_089E3208(unsigned char *object)
{
    ((BattleTableAt1C0 *)object)->member_table = D_089E38C0;
    func_08993560(object + 8, 2, 0xD8, D_089E38D0);
    func_089E32D0(object);
    return object;
}
