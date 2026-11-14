#include "menu/agent29_types.h"

/* Screen0B6A0: once the pull's models are done, closes the help line,
   stops the layout, releases the page and frees all 0x92 text objects
   (the texts run on past the region header's 0x71), then moves to
   state 0xF. */
extern void func_089FAE70(Screen0B6A0 *self);
extern int func_089FAEB0(Screen0B6A0 *self);
extern int func_089FBF94(Screen0B6A0 *self);
extern void func_089FADF4(Screen0B6A0 *self);

void func_089F9EE0(Screen0B6A0 *self)
{
    int i;

    func_089FAE70(self);
    if (func_089FAEB0(self) == 0) {
        return;
    }
    if (func_089FBF94(self) == 0) {
        return;
    }
    func_08885220(D_089D69E8);
    func_088D3A04(&self->work->layout);
    func_089FADF4(self);
    for (i = 0; i < 0x92; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    self->work->state = 0xF;
}
