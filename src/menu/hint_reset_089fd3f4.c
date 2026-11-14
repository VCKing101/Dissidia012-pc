/* Copies the hint start value into both totals and restarts the hint timer. */
#include "menu/agent16_types.h"

extern void func_088D4A68(void *timer, void *source, int a, int b, int one, int frames, int loop);
extern void func_089FD474(A16HintOwner *self);

void func_089FD3F4(A16HintOwner *self)
{
    A16HintState *state = self->field_14C;
    A16HintMarks *marks = &state->u.hint.field_38A0;
    int start = marks->field_112;
    state->u.totals.field_33C44 = start;
    self->field_14C->u.totals.field_33C50 = start;
    func_088D4A68(marks->field_303CC, marks->field_303A0, marks->field_1C, marks->field_14, 1, 0x14, 1);
    func_089FD474(self);
}
