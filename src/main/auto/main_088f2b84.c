#include "main/auto_region4.h"

/* Counts id in field_1570 when its record has flag 0x20. */
void func_088F2B84(Obj8F2B84 *self, int id)
{
    Record8C0F8C *record;

    record = func_088C0F8C(id);
    if ((record != 0) && ((record->flags & 0x20) != 0)) {
        self->field_1570 = self->field_1570 + 1;
    }
}
