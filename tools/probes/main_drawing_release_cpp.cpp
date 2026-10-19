extern "C" {
#include "main/drawing_record.h"
}
extern "C" void func_088DA060(MainDrawingRecordPrefix *self) {
    if (self->field_0024) func_088DAAA0(self->field_0024,self);
    self->field_0024 = 0;
}
