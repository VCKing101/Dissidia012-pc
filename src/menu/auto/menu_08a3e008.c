#include "menu/auto_region2.h"

extern const short D_08A40260[];

/* Screen40248: releases its texts and model handles, resets the root's
 * prompt, stops its layout scripts and moves to state 3. */
void func_08A3E008(Screen40248 *self)
{
    int i;

    for (i = 0; i < 0x20; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    for (i = 0; i < 0x4C; i++) {
        if (self->work->handles[i] != 0) {
            func_088DA060(self->work->handles[i]);
            self->work->handles[i] = 0;
        }
    }
    func_08A418E8(D_08A457EC);
    func_088D3A5C(self->work->layout, D_08A40260, 0x13);
    self->work->field_2A04 = 0;
    self->work->state = 3;
}
