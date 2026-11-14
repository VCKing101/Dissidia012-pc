#include "battle/auto_region4.h"

/* PartA4CA0: removes list position `slot`; its slot gets state 2 and the
 * last listed index takes the position. */
void func_08AA4C38(PartA4CA0 *self, int slot)
{
    self->slots[self->list[slot]].state = 2;
    self->list[slot] = self->list[self->count - 1];
    self->count = self->count - 1;
}
