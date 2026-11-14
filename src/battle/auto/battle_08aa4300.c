#include "battle/auto_region4.h"

/* PartA4CA0 reset. */
void func_08AA4300(PartA4CA0 *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->slots = 0;
    self->list = 0;
    self->count = 0;
    self->added = 0;
    self->history = 0;
    self->field_20 = 0;
}
