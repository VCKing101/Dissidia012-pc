#include "menu/auto_region1.h"

/* Screen0B6A0: resets fields 3418..3430 and clears flag 0x20 of field_3280. */
void func_089FAED0(Screen0B6A0 *self)
{
    self->work->field_3418 = 0;
    self->work->field_341C = 0;
    self->work->field_3420 = 0;
    self->work->field_3428 = -1;
    self->work->field_3430 = 0;
    self->work->field_3280 = self->work->field_3280 & ~0x20;
}
