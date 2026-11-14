#include "menu/auto_region1.h"

/* Screen1AB08: keeps func_088D5AC8's result in field_16A4 and copies
 * field_1124/field_1128 to field_16A0/field_16A8. */
void func_08A1938C(Screen1AB08 *self)
{
    self->work->field_16A4 = func_088D5AC8(&self->work->field_1540, 0x23, 0, 0);
    self->work->field_16AC = 5;
    self->work->field_16A0 = self->work->field_1124;
    self->work->field_16A8 = self->work->field_1128;
}
