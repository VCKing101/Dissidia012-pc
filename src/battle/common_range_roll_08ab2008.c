#include "battle/agent15_types.h"

/* Random value inside the battle-wide range at 0x64. */
float func_08AB2008(UnitParamOwner *self)
{
    BattleCommonParams *params;
    float min;
    float max;
    if (self->active == 0) {
        return 0.0f;
    }
    params = func_08AACB10(func_08A80200(D_08A80218));
    if (params == 0) {
        return 0.0f;
    }
    min = params->range_64.min;
    max = params->range_64.max;
    return (max - min) * func_08AA57D0(D_08AA5828) + min;
}
