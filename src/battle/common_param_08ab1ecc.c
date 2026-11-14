#include "battle/agent15_types.h"

/* Battle-wide value at 0x54, 0 when the owner is inactive. */
float func_08AB1ECC(UnitParamOwner *self)
{
    BattleCommonParams *params;
    if (self->active == 0) {
        return 0.0f;
    }
    params = func_08AACB10(func_08A80200(D_08A80218));
    if (params == 0) {
        return 0.0f;
    }
    return params->field_54;
}
