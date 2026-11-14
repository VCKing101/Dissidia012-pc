#include "battle/auto_region4.h"

/* Part956C8: marks entry `index` as listed (state 1) and appends it to the
 * list. */
void func_08A955B4(Part956C8 *self, int index)
{
    self->entries[index].state = 1;
    self->list[self->count] = index;
    self->count = self->count + 1;
}
