#include "main/drawing_record.h"

void func_088D9F08(MainDrawingRecordPrefix *self, float value) {
    float current;
    self->field_0004 = value;
    current = self->field_0000;
    if (!(value <= current)) self->field_0020 &= ~4;
    else if (value < current) self->field_0020 |= 4;
}
