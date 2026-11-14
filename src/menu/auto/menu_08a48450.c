#include "menu/auto_region2.h"

/* IdTable: id in column 0..3 of row index, -1 for another column. */
int func_08A48450(IdTable *self, int index, int column)
{
    int id;

    id = -1;
    if (column == 0) {
        id = self->rows[index].field_00;
    } else if (column == 1) {
        id = self->rows[index].field_02;
    } else if (column == 2) {
        id = self->rows[index].field_04;
    } else if (column == 3) {
        id = self->rows[index].field_06;
    }
    return id;
}
