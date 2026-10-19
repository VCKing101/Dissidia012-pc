#include "main/drawing_record.h"

void func_088D9CD8(MainDrawingRecordPrefix *self) {
    self->field_0020 |= 8;
    func_088D9D24(self);
}
void func_088D9CFC(MainDrawingRecordPrefix *self) {
    self->field_0020 &= ~8;
    func_088D9D24(self);
}

