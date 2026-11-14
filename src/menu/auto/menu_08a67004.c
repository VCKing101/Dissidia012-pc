#include "menu/auto_region2.h"

/* Panel68180: clears field_50 and the first shown row. */
void func_08A67004(Panel68180 *self)
{
    self->field_50 = 0;
    self->scroll.field_04 = 0;
}
