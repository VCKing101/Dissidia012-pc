#include "menu/auto_region1.h"

/* Screen0B6A0: field_06 of record id. */
int func_089FA884(Screen0B6A0 *self, int id)
{
    Record8CB164 *record;

    record = func_088CB164(D_089D712C->field_F58, id);
    return record->field_06;
}
