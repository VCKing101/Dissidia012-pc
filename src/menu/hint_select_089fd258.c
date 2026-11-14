#include "menu/agent3_types.h"

void func_089FD258(MenuHintOwner *self, short value) {
    MenuHintMarks *marks = &self->field_14C->field_38A0;
    int i;

    for (i = 0; i < marks->field_26D64; i++) {
        marks->field_1544[i].field_01 = marks->field_1544[i].field_06 == value;
    }
}
