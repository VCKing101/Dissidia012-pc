#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089E6E50[];
extern void func_089E1D00(void *);
extern void func_089E9E10(void *);
extern void func_089E6998(void *);

void *func_089E68E0(unsigned char *object)
{
    ((BattleTableAt11DC *)object)->member_table = D_089E6E50;
    func_089E1D00(object + 8);
    func_089E9E10(object + 0x124);
    func_089E6998(object);
    return object;
}
