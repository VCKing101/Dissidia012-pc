#include "menu/agent29_types.h"

/* ScreenE75C4: once any of handles 0x16, 0x1F or 0x2A reaches motion state
   0xD, starts a fade to black, clears flag 4 and moves to state 0xC. */
extern char D_089E0A88[];

void func_089E14B8(ScreenE75C4 *self)
{
    FadeColor29 color;

    func_089E1CA4(self);
    func_089E1DD8(self);
    func_089E21C8(self);
    if (self->work->handles[0x16]->field_0C != 0xD && self->work->handles[0x1F]->field_0C != 0xD &&
        self->work->handles[0x2A]->field_0C != 0xD) {
        return;
    }
    func_08873D34((float)func_088D4C5C(), 2, D_089E0A88, 0, 1, 2);
    color.r = 0.0f;
    color.g = 0.0f;
    color.b = 0.0f;
    color.a = 0.0f;
    func_08873DB0(&color, 2);
    self->work->flags &= ~4;
    self->work->state = 0xC;
}
