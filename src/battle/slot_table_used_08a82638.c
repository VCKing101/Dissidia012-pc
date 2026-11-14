#include "battle/agent14_types.h"

int func_08A82638(SlotTable *self, unsigned int index)
{
    SlotEntry *entries;

    if (self->active == 0) {
        return 0;
    }
    if (index >= 34) {
        return 0;
    }
    entries = self->entries;
    return entries[index].used == 1;
}
