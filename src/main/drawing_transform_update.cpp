#include "main/drawing_flag_accessors.h"

extern "C" void func_088D9B84(MainDrawingRecordPrefix *self, float *values) {
    if (values[0] != 1.0f || values[5] != 1.0f) MainDrawingFlagAccessors::clear_flag(self,0x400);
    else MainDrawingFlagAccessors::set_flag(self,0x400);
    self->field_0018 = values[0];
    self->field_001C = values[5];
    self->field_0010 = values[12];
    self->field_0014 = values[13];
    MainDrawingFlagAccessors::clear_flag(self,0x800);
}
