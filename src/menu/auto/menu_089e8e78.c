#include "menu/auto_region1.h"

/* ScreenE9D38: sets field_13C's byte 0x15C, then field_154 = 2. */
void func_089E8E78(ScreenE9D38 *self)
{
    self->field_13C->field_15C = 1;
    self->field_154 = 2;
}
