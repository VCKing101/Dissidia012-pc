#include "menu/auto_region1.h"

/* Screen0B6A0: field_10 of row index of field_44, 0 past the row count. */
int func_089EF8F4(Screen0B6A0 *self, char index)
{
    int value;

    value = 0;
    if ((int)index < (int)self->work->field_40->field_10) {
        value = self->work->field_44[index].field_10;
    }
    return value;
}
