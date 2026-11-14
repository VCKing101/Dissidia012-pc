#include "menu/auto_region1.h"

/* Screen0B6A0: how much of count is left over the field_0C of record id. */
int func_089FA83C(Screen0B6A0 *self, int id, int count)
{
    Record8CB164 *record;
    int value;

    record = func_088CB164(D_089D712C->field_F58, id);
    value = record->field_0C;
    if (count < record->field_0C) {
        value = count;
    }
    return count - value;
}
