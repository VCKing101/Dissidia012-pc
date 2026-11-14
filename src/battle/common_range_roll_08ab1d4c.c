#include "battle/agent15_types.h"

/* Random value inside the battle-wide range at 0x3C. */
float func_08AB1D4C(UnitParamOwner *self)
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
    min = params->range_3C.min;
    max = params->range_3C.max;
    return (max - min) * func_08AA57D0(D_08AA5828) + min;
}
