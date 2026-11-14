#include "menu/auto_region2.h"

/* BlockView constructor: clears it and makes it D_08A49748. */
BlockView *func_08A49610(BlockView *self)
{
    self->header = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    D_08A49748 = self;
    return self;
}
