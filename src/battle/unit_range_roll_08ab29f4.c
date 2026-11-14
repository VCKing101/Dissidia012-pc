#include "battle/agent15_types.h"

typedef struct IdRange {
    int id;
    FloatRange range;
} IdRange;

extern int func_08AAE300(UnitParamOwner *self);
extern IdRange *func_08AAC380(UnitParamTable *table, int id);

/* Random value inside the range stored for the owner's second id. */
float func_08AB29F4(UnitParamOwner *self)
{
    IdRange *entry;
    float min;
    float max;
    if (self->active == 0) {
        return 0.0f;
    }
    entry = func_08AAC380(func_08A80200(D_08A80218), func_08AAE300(self));
    if (entry == 0) {
        return 0.0f;
    }
    min = entry->range.min;
    max = entry->range.max;
    return (max - min) * func_08AA57D0(D_08AA5828) + min;
}
