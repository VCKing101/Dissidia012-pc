#include "menu/agent29_types.h"

/* ScreenE75C4 leave: once flag 2 is set, frees the layout, the texts and
   the messages, then moves to state 6. */
extern "C" {

void func_089E133C(ScreenE75C4 *self)
{
    int i;

    if (!Flags29::Has(self->work->flags, 2)) {
        return;
    }
    func_089E2CD0(self);
    func_088D3A04(&self->work->layout);
    for (i = 0; i < 0x59; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    if (self->work->messages != 0) {
        func_0887B1F4(self->work->messages, 3);
        self->work->messages = 0;
    }
    func_08885220(D_089D69E8);
    func_088F0A78(1);
    self->work->state = 6;
}

}
