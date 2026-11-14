#include "battle/auto_region4.h"

/* Part956C8: removes list slot `slot`; its entry gets state 2 and the last
 * listed index takes the slot. */
void func_08A955F4(Part956C8 *self, int slot)
{
    self->entries[self->list[slot]].state = 2;
    self->list[slot] = self->list[self->count - 1];
    self->count = self->count - 1;
}
