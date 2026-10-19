/* Uncredited probes: branch forms, register selection and flag access order
 * differ. No volatile qualification is claimed in the production layout. */
#include "main/drawing_record.h"

void func_088D9B84(void *object, void *buffer) {
    MainDrawingRecordPrefix *self = object;
    float *values = buffer;
    if (values[0] != 1.0f || values[5] != 1.0f) self->field_0020 &= ~0x400;
    else self->field_0020 |= 0x400;
    self->field_0018 = values[0];
    self->field_001C = values[5];
    self->field_0010 = values[12];
    self->field_0014 = values[13];
    self->field_0020 &= ~0x800;
}

void func_088D9C60(MainDrawingRecordPrefix *self, int a, int b) {
    float first = (float)a;
    float second = (float)b;
    self->field_0020 &= ~0x800;
    self->field_0010 = first;
    self->field_0014 = second;
}
