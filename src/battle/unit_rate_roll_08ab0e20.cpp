#include "battle/agent23_types.h"

/* 1 when a random roll falls below the owner's unit parameter rate at 0x28. */
extern "C" unsigned char func_08AB0E20(UnitParamOwner *self)
{
    UnitParamTable *table;
    int id;
    UnitRateEntry *entry;
    if (self->active == 0) {
        return 0;
    }
    table = func_08A80200(D_08A80218);
    id = func_08AAE41C(self);
    entry = (UnitRateEntry *)func_08AACA24(table, id);
    if (entry == 0) {
        return 0;
    }
    return UnitRoll::below(entry->rate_28);
}
