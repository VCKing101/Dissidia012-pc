#include "main/drawing_record.h"

void func_088D9DDC(MainDrawingRecordPrefix *self) {
    if (self->field_0028[0] == 0x80 && self->field_0028[1] == 0x80 &&
        self->field_0028[2] == 0x80 && self->field_0028[3] == 0xFF) {
        self->field_0020 |= 0x200;
    } else self->field_0020 &= ~0x200;
    func_088D9D24(self);
}
