#include "menu/agent29_types.h"

/* Screen15A88: unless the board is busy, drops flag 0x20000, marks the
   piece under the cursor (+0xEE8 column, +0xEEC row) when the root
   allows it, and moves on to step 0xCD. */
extern int func_08A1311C(Screen15A88 *self);
extern int func_08A1F6F8(MenuRoot *root);
extern void func_08A13B64(Screen15A88 *self, int arg);
extern void func_08A13D40(Screen15A88 *self);

void func_08A136A0(Screen15A88 *self)
{
    int cell;

    if (func_08A1311C(self) == 0) {
        self->work->flags &= ~0x20000;
        cell = WORK_INT(self->work, 0xEEC) + WORK_INT(self->work, 0xEE8) * 8;
        if (func_08A1F6F8(D_08A23FF0) == 1) {
            (&WORK_BYTE(self->work, 0xCE8))[(&WORK_SHORT(self->work, 0xF00))[cell] * 0x1E] |= 4;
        }
        self->work->field_EDC = 0xCD;
        func_08A13B64(self, 1);
        func_08A13D40(self);
    }
}
