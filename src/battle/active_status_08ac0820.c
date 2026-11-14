#include "battle/agent15_types.h"

/* 0 for an active owner, -1 otherwise. */
int func_08AC0820(UnitParamOwner *self)
{
    if (self->active == 0) {
        return -1;
    }
    return 0;
}
