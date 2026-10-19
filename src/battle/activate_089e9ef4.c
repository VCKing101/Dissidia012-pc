#include "battle/runtime_helpers.h"

/* Connected activation/state helpers; complete class ownership is unknown. */


void func_089E9EF4(unsigned char *object)
{
    object[0] = 0;
    ((BattleActivationHeader *)object)->field_04 = 0;
    ((BattleActivationHeader *)object)->field_18 = 0;
}
