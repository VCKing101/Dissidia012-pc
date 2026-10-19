#include "main/drawing_record.h"

void func_088D9C0C(MainDrawingRecordPrefix *self, float *values) {
    self->field_002C = values;
    self->field_0018 = 1.0f;
    self->field_001C = 1.0f;
    self->field_0020 |= 0x400;
    self->field_0010 = values[12];
    self->field_0014 = values[13];
    values[12] = 0.0f;
    self->field_002C[13] = 0.0f;
    self->field_0020 &= ~0x800;
}

