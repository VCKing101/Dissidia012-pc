#include "battle/agent14_types.h"

float func_08A82764(SlotTable *self, unsigned int index)
{
    SlotEntry *entries;

    if (self->active == 0) {
        return 0.0f;
    }
    if (index >= 34) {
        return 0.0f;
    }
    entries = self->entries;
    return entries[index].time;
}
