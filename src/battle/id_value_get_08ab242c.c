#include "battle/agent15_types.h"

/* The parameter table's value for id, 0 when there is none. */
float func_08AB242C(UnitParamOwner *self, int id)
{
    IdValue *entry;
    if (self->active == 0) {
        return 0.0f;
    }
    if (id < 0) {
        return 0.0f;
    }
    entry = func_08AACB30(func_08A80200(D_08A80218), id);
    if (entry == 0) {
        return 0.0f;
    }
    return entry->value;
}
