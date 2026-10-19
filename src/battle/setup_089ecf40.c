#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089ED850[];
extern void func_089ECFD0(void *);

void *func_089ECF40(unsigned char *object)
{
    ((BattleTableAt20 *)object)->member_table = D_089ED850;
    func_089ECFD0(object);
    return object;
}
