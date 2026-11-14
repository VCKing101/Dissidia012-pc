#include "battle/agent15_types.h"

extern int func_08AAE300(UnitParamOwner *self);
extern void *func_08AACE60(UnitParamTable *table, int id);

/* 1 when the parameter table has an entry for the owner's second id. */
int func_08AB249C(UnitParamOwner *self)
{
    int id;
    if (self->active == 0) {
        return 0;
    }
    id = func_08AAE300(self);
    if (id < 0) {
        return 0;
    }
    return func_08AACE60(func_08A80200(D_08A80218), id) != 0;
}
