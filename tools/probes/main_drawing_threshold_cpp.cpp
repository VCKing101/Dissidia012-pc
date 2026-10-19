#include "main_drawing_helpers.hpp"
extern "C" void func_088D9F08(MainDrawingRecordPrefix *self, float value) {
    self->field_0004 = value;
    float selected = self->field_0004;
    float current = self->field_0000;
    if (selected <= current) goto selected_not_greater;
    main_drawing_clear_flag(self,4);
    return;
selected_not_greater:
    if (selected < current) main_drawing_set_flag(self,4);
}
