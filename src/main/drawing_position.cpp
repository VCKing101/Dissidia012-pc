#include "main/drawing_flag_accessors.h"

extern "C" void func_088D9C60(MainDrawingRecordPrefix *self, int a, int b) {
    float first = (float)a;
    float second = (float)b;
    MainDrawingFlagAccessors::clear_flag(self,0x800);
    self->field_0010 = first;
    self->field_0014 = second;
}
