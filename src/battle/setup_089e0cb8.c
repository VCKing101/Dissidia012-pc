#include "battle/runtime_helpers.h"

/* Constructor-like setup path; names and complete class layouts remain unknown. */
extern unsigned char D_089E1060[];

void *func_089E0CB8(unsigned char *object)
{
    ((BattleTableAt8 *)object)->member_table = D_089E1060;
    func_089E0D48(object);
    return object;
}
