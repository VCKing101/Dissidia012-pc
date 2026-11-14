#include "menu/auto_region1.h"

/* Screen1AB08: stores field_16E0 in entry field_16B4 of field_1680. */
void func_08A18E80(Screen1AB08 *self)
{
    Work1AB08 *work;

    work = self->work;
    work->field_1680[work->field_16B4].field_00 = work->field_16E0;
}
