#include "battle/runtime_helpers.h"

/* Connected activation/state helpers; complete class ownership is unknown. */
extern int func_089E21D0(void *);

int func_089E20FC(unsigned char *object, unsigned int index)
{
    if (!object[0]) return -1;
    if (index >= 3) return -2;
    ((BattleSlots114 *)object)->field_114[index] = 0;
    if (func_089E21D0(object)) return -3;
    return 0;
}

int func_089E2164(unsigned char *object, unsigned int index)
{
    if (!object[0]) return -1;
    if (index >= 3) return -2;
    ((BattleSlots114 *)object)->field_114[index] = 1;
    if (func_089E21D0(object)) return -3;
    return 0;
}
