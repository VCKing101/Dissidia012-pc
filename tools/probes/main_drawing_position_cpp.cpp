#include "main_drawing_helpers.hpp"
extern "C" void func_088D9C60(MainDrawingRecordPrefix *self, int a, int b) {
    float first = (float)a;
    float second = (float)b;
    main_drawing_clear_flag(self,0x800);
    self->field_0010 = first;
    self->field_0014 = second;
}
