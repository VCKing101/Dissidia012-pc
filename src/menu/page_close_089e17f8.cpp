#include "menu/agent29_types.h"

/* ScreenE75C4: when the page has finished closing, frees the layout and
   texts and moves to state 0xF (back to the field on page 1). */
extern "C" {
extern const short D_089E6606[];
int func_089E30F8(ScreenE75C4 *self);
int func_08885608(void *object, char value);
void func_089E303C(ScreenE75C4 *self);
void func_089E30B8(ScreenE75C4 *self);

void func_089E17F8(ScreenE75C4 *self)
{
    int i;
    int none = 0;

    func_089E30B8(self);
    if (func_089E30F8(self) == 0) {
        return;
    }
    if (func_08885608(D_089D69E8, none) == 0) {
        return;
    }
    if (!Flags29::Has(self->work->flags, 2)) {
        return;
    }
    func_088D3C24(&self->work->layout, self->work->handles, D_089E6606, 0x120);
    func_088D3A04(&self->work->layout);
    func_089E303C(self);
    for (i = 0; i < 0x59; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    func_08885220(D_089D69E8);
    if (WORK_INT(self->work, 0xB1C) == 1) {
        int one = 1;
        func_088F0A78(one);
    }
    self->work->state = 0xF;
}
}
