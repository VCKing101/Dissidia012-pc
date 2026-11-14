/* Places hint mark 1 at the stored cell (scaled by 11) and refreshes the marks. */
#include "menu/agent16_types.h"

extern void func_089FD1F0(A16HintOwner *self, signed char id, short x, short y);
extern void func_089FD2A8(A16HintOwner *self);

void func_089FE448(A16HintOwner *self)
{
    A16HintMarks *marks = &self->field_14C->u.hint.field_38A0;
    short x = marks->field_303E8 * 11;
    short y = marks->field_303EA * 11;
    func_089FD1F0(self, 1, x, y);
    func_089FD2A8(self);
}
