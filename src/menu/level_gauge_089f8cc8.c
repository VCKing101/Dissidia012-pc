#include "menu/agent29_types.h"

/* Screen0B6A0: once (page flag 2 of +0x3238), opens the level gauge:
   plays its script, sets the gauge model (0xFA) to state 9 and hides it
   and its three marks, takes the level (+0x279D, at most 100) and its
   step count (1 for none, the level below 100, else level / 9 + 1),
   starts sound 0x15 and moves on. */
extern const unsigned short D_08A0A212[];
extern void func_089F8E18(Screen0B6A0 *self);

void func_089F8CC8(Screen0B6A0 *self)
{
    short level;
    short steps;
    int i;

    if (WORK_INT(self->work, 0x3238) & 2) {
        return;
    }
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A0A212);
    MENU_MODEL_STATE(self->work->handles[0xFA]) = 9;
    level = WORK_BYTE(self->work, 0x279D);
    if (level > 100) {
        level = 100;
    }
    for (i = 0; i < 3; i++) {
        func_088D9CFC(self->work->handles[0xF7 + i]);
    }
    func_088D9CFC(self->work->handles[0xFA]);
    WORK_INT(self->work, 0x3254) = level;
    steps = 1;
    if (level != 0) {
        steps = level;
        if (steps >= 100) {
            steps = level / 9 + 1;
        }
    }
    WORK_SHORT(self->work, 0x3258) = steps;
    WORK_SHORT(self->work, 0x325A) = 0;
    WORK_INT(self->work, 0x325C) = func_088D4BF4(0x15);
    WORK_BYTE(self->work, 0x31E4) = 0;
    WORK_INT(self->work, 0x3238) |= 2;
    func_089F8E18(self);
}
