#include "menu/agent3_types.h"

void func_089FD18C(MenuHintOwner *self, signed char a, short b, short c, signed char d, short e) {
    MenuHintMarks *marks = &self->field_14C->field_38A0;
    int count = marks->field_26D64;

    marks->field_1544[count].field_00 = a;
    marks->field_1544[count].field_01 = d;
    marks->field_1544[count].field_02 = b;
    marks->field_1544[count].field_04 = c;
    marks->field_1544[count].field_06 = e;
    marks->field_26D64 = count + 1;
}
