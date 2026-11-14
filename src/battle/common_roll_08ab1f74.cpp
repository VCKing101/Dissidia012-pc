#include "battle/agent23_types.h"

struct CommonRates {
    char pad_00[0x5C];
    float rate_5C;
};

extern "C" {

/* 1 when a random roll falls below the battle-wide rate at 0x5C. */
unsigned char func_08AB1F74(UnitParamOwner *self)
{
    CommonRates *params;
    if (self->active == 0) {
        return 0;
    }
    params = (CommonRates *)func_08AACB10(func_08A80200(D_08A80218));
    if (params == 0) {
        return 0;
    }
    return UnitRoll::below(params->rate_5C);
}

}
