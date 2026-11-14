#include "menu/auto_region3.h"

/* StateQueue: sets the state the current slot enters next. */
void func_08A79E68(StateQueue *self, int next)
{
    self->slots[self->index].next = next;
}
