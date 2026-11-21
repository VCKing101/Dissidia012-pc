#include "menu/agent36_types.h"

/* Screen28560: counts the close timer down (closing at zero), then plays
 * the eight rows' layout and text scripts and the cursor. */
void func_08A247AC(Screen28560x *self)
{
    int i;

    if (self->work->timer != 0) {
        self->work->timer--;
        if (self->work->timer <= 0) {
            self->work->timer = 0;
            func_08A245C0(self);
            func_08A26C48(self, 1);
        }
    }
    for (i = 0; i < 8; i++) {
        func_088D3F00(self->work->layout, self->work->handles, self->work->scripts[i]);
        func_088D44EC(self->work->handles, self->work->texts, self->work->text_scripts[i]);
    }
    func_088D4B74(self->work->field_2070, 0);
    func_08A246F4(self);
}
