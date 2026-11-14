#include "battle/auto_region4.h"

/* PartA5830: removes list slot `slot`; its entry gets state 2 and the last
 * listed index takes the slot. */
void func_08AA6228(PartA5830 *self, int slot)
{
    self->entries[self->list[slot]].state = 2;
    self->list[slot] = self->list[self->count - 1];
    self->count = self->count - 1;
}
