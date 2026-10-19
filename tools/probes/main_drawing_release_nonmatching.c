#include "main/drawing_record.h"

void func_088DA060(MainDrawingRecordPrefix *self) {
    if (!self->field_0024) {
    } else func_088DAAA0(self->field_0024, self);
    self->field_0024 = 0;
}
