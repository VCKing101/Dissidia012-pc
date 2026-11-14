#include "menu/auto_region1.h"

/* Screen0B6A0: points field_08..field_3C at the header and the tables of
 * the data block in field_04, each table after the one before (row counts
 * from the header). */
void func_08A05B0C(Screen0B6A0 *self)
{
    char *cursor;

    cursor = self->work->field_04;
    self->work->header = (MenuDataHeader *)cursor;
    cursor = cursor + sizeof(MenuDataHeader);
    self->work->field_0C = cursor;
    cursor = cursor + self->work->header->field_10 * 0x20;
    self->work->field_10 = cursor;
    cursor = cursor + self->work->header->field_12 * 0x20;
    self->work->field_14 = cursor;
    cursor = cursor + self->work->header->field_14 * 0x10;
    self->work->field_18 = cursor;
    cursor = cursor + self->work->header->field_16 * 0x20;
    self->work->field_1C = cursor;
    cursor = cursor + self->work->header->field_18 * 0x20;
    self->work->field_20 = cursor;
    cursor = cursor + self->work->header->field_1A * 0x40;
    self->work->field_24 = cursor;
    cursor = cursor + self->work->header->field_28 * 0x18;
    self->work->field_28 = cursor;
    cursor = cursor + self->work->header->field_1C * 0x68;
    self->work->field_2C = cursor;
    cursor = cursor + self->work->header->field_1E * 8;
    self->work->field_30 = cursor;
    cursor = cursor + self->work->header->field_20 * 0x118;
    self->work->field_34 = cursor;
    cursor = cursor + self->work->header->field_22 * 0x30;
    self->work->field_38 = cursor;
    self->work->field_3C = cursor + self->work->header->field_24 * 0x24;
}
