#include "battle/auto_region5.h"

/* PartFBE40: sets field_24 and clears field_2D. */
void func_08AFBE40(PartFBE40 *self, int value)
{
    self->field_24 = value;
    self->field_2D = 0;
}
