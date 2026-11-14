#include "battle/auto_region4.h"

/* PartAAD68 reset: the first PartA76A0 becomes the current one. */
void func_08AA7840(PartAAD68 *self)
{
    self->active = 0;
    self->current = &self->field_08;
}
