#include "battle/agent15_types.h"

/* Field 0x130 of the owner's unit parameter entry, -1 when there is none. */
int func_08AB1248(UnitParamOwner *self)
{
    UnitParamTable *table;
    int id;
    UnitParamEntry *entry;
    if (self->active == 0) {
        return -1;
    }
    table = func_08A80200(D_08A80218);
    id = func_08AAE41C(self);
    entry = func_08AACA24(table, id);
    if (entry == 0) {
        return -1;
    }
    return entry->field_130;
}
