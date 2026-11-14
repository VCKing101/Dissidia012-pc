#include "menu/auto_region1.h"

/* Board: clears field_6B3F4 and keeps value in field_6B400. */
void func_08A28C08(Board *self, int value)
{
    self->field_6B3F4 = 0;
    self->field_6B400 = value;
}
