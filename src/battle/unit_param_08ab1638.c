#include "battle/agent15_types.h"

/* Field 0x30 of the owner's unit parameter entry, 0 when there is none. */
int func_08AB1638(UnitParamOwner *self)
{
    UnitParamTable *table;
    int id;
    UnitParamEntry *entry;
    if (self->active == 0) {
        return 0;
    }
    table = func_08A80200(D_08A80218);
    id = func_08AAE41C(self);
    entry = func_08AACA24(table, id);
    if (entry == 0) {
        return 0;
    }
    return entry->field_30;
}
