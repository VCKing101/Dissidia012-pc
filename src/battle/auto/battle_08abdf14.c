#include "battle/auto_region4.h"

/* PartC03B8 reset. */
void func_08ABDF14(PartC03B8 *self)
{
    int i;
    ListHead *list;

    self->active = 0;
    self->field_04 = 0;
    self->field_08 = 0;
    self->field_0C = 0;
    self->field_10 = 0;
    self->field_14 = 0;
    self->field_18 = 0;
    self->field_1C.field_04 = 0;
    self->field_1C.first = 0;
    self->field_1C.field_08 = 0;
    self->field_28.field_04 = 0;
    self->field_28.first = 0;
    self->field_28.field_08 = 0;
    self->field_34.field_04 = 0;
    self->field_34.first = 0;
    self->field_34.field_08 = 0;
    self->field_40.field_04 = 0;
    self->field_40.first = 0;
    self->field_40.field_08 = 0;
    for (i = 0, list = self->field_4C; i < 3; i++) {
        list[i].field_04 = 0;
        list[i].first = 0;
        list[i].field_08 = 0;
        self->field_70[i] = 0;
    }
}
