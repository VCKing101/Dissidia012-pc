#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089E70D8[];

void *func_089E6E60(unsigned char *object)
{
    ((BattleTableAt8 *)object)->member_table = D_089E70D8;
    func_089E6EF0(object);
    return object;
}
