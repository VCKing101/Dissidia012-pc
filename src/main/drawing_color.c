#include "main/drawing_record.h"

void func_088D9AF4(void *object, void *value) {
    MainDrawingRecordPrefix *self = object;
    func_08935524(self->field_0028, value, 4);
    func_088D9DDC(self);
}

void func_088D9B28(MainDrawingRecordPrefix *self, unsigned char value) {
    self->field_0028[3] = value;
    func_088D9DDC(self);
}

void func_088D9B48(MainDrawingRecordPrefix *self, unsigned int value) {
    func_08935524(self->field_0028, &value, 4);
    func_088D9DDC(self);
}
