/* Moves the hint mark with this id to (x, y). */
#include "menu/agent16_types.h"
void func_089FD1F0(A16HintOwner *self, signed char id, short x, short y)
{
    A16HintMarks *marks = &self->field_14C->u.hint.field_38A0;
    int i;
    for (i = 0; i < marks->field_26D64; i++) {
#if 1 == 1
        if (marks->field_1544[i].field_00 == id) {
            marks->field_1544[i].field_02 = x;
            marks->field_1544[i].field_04 = y;
            break;
        }
#else
        A16HintMark *mark = &marks->field_1544[i];
        if (mark->field_00 == id) {
            mark->field_02 = x;
            mark->field_04 = y;
            return;
        }
#endif
    }
}
