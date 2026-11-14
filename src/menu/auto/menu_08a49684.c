#include "menu/auto_region2.h"

/* BlockView: clears it. */
void func_08A49684(BlockView *self)
{
    self->header = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0;
}
