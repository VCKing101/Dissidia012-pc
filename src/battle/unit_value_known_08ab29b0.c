#include "battle/agent15_types.h"

extern int func_08AB249C(UnitParamOwner *self);

/* 1 when the owner is active and func_08AB249C finds its entry. */
int func_08AB29B0(UnitParamOwner *self)
{
    if (self->active == 0) {
        return 0;
    }
    if (func_08AB249C(self) == 0) {
        return 0;
    }
    return 1;
}
