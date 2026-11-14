#include "battle/auto_region4.h"

/* PartA5830: if entry `index` is not listed (state 1), marks it and
 * appends it to the list. */
void func_08AA61D8(PartA5830 *self, int index)
{
    EntryA5830 *entry;

    entry = &self->entries[index];
    if (entry->state != 1) {
        entry->state = 1;
        self->list[self->count] = index;
        self->count = self->count + 1;
    }
}
