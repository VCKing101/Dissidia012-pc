#include "menu/auto_region1.h"

/* Screen0B6A0: moves cursor field_3370 to min(field_2731, field_2732) and
 * calls func_089F309C when it moved. */
void func_089F7634(Screen0B6A0 *self)
{
    int value;
    Work0B6A0 *work;

    work = self->work;
    value = work->field_2731;
    if ((int)work->field_2732 < (int)work->field_2731) {
        value = work->field_2732;
    }
    value = func_088D484C(&work->field_3370, value, 0);
    if (value != 0) {
        func_089F309C(self);
    }
}
