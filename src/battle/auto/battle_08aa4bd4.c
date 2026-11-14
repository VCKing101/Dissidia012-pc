#include "battle/auto_region4.h"

/* PartA4CA0: if slot `index` is unused, marks it listed (state 1) and
 * appends it to both lists. */
void func_08AA4BD4(PartA4CA0 *self, int index)
{
    SlotA4CA0 *slot;

    slot = &self->slots[index];
    if (slot->state == 0) {
        slot->state = 1;
        self->list[self->count] = index;
        self->count = self->count + 1;
        self->history[self->added] = index;
        self->added = self->added + 1;
    }
}
