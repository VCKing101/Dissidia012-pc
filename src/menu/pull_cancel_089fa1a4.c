#include "menu/agent29_types.h"

/* Screen0B6A0: once the prompt is done, closes the help
   line, hides the models, frees all 0x92 text objects (past the region
   header's 0x71), saves and moves to state 0x12. */
extern void func_089FAE70(Screen0B6A0 *self);
extern int func_089FAEB0(Screen0B6A0 *self);
extern int func_089FB568(Screen0B6A0 *self, void *layout, void *handles, void *texts);
extern void func_088D3CB0(void *handles, int count);
extern void func_088FE9CC(void);

void func_089FA1A4(Screen0B6A0 *self)
{
    int i;

    func_089FAE70(self);
    if (func_089FAEB0(self) == 0) {
        return;
    }
    if (func_089FB568(self, &self->work->layout, self->work->handles, self->work->texts) == 0) {
        return;
    }
    func_08885220(D_089D69E8);
    func_088D3CB0(self->work->handles, 0x180);
    for (i = 0; i < 0x92; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
    func_088FE9CC();
    self->work->state = 0x12;
}
