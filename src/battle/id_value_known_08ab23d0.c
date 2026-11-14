#include "battle/agent15_types.h"

/* 1 when the parameter table holds a value for id. */
int func_08AB23D0(UnitParamOwner *self, int id)
{
    if (self->active == 0) {
        return 0;
    }
    if (id < 0) {
        return 0;
    }
    return func_08AACB30(func_08A80200(D_08A80218), id) != 0;
}
