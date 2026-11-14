#include "menu/auto_region1.h"

/* Screen0B6A0: field_2730 - 1, at least 0 (the current entry). */
int func_089F2FF0(Screen0B6A0 *self)
{
    int index;

    index = self->work->field_2730 - 1;
    if (index < 0) {
        index = 0;
    }
    return index;
}
