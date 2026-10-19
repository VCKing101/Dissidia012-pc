#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089E8F58[];

void *func_089E8E70(unsigned char *object)
{
    ((BattleTableAt4 *)object)->member_table = D_089E8F58;
    func_089E8EDC((unsigned int *)object);
    return object;
}
