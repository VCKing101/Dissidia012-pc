#include "menu/agent29_types.h"

/* MenuRoot: closes the root's page once the network is idle (not in
   state 1 or 3) and func_08A1DA90 agrees: saves when flag 0x10 asks for
   it, frees the page's 0x1B texts (+0x890), closes the help line and
   moves to step 6. */
extern int func_08A1D90C(MenuRoot *self);
extern int func_08A1DA90(MenuRoot *self);

#define ROOT_TEXT(work, i) (((void **)&WORK_INT(work, 0x890))[i])

void func_08A1B7C4(MenuRoot *self)
{
    int state = func_08A1D90C(self);
    int i;

    if (state == 1 || state == 3) {
        return;
    }
    if (func_08A1DA90(self) == 0) {
        return;
    }
    if (WORK_INT(self->work, 0xD80) & 0x10) {
        int none = 0;
        func_088AC8C8(D_089D712C, none);
        WORK_INT(self->work, 0xD80) &= ~0x10;
    }
    for (i = 0; i < 0x1B; i++) {
        if (ROOT_TEXT(self->work, i) != 0) {
            func_0887BA5C(ROOT_TEXT(self->work, i));
            ROOT_TEXT(self->work, i) = 0;
        }
    }
    func_08885220(D_089D69E8);
    WORK_INT(self->work, 0xD7C) = 6;
}
