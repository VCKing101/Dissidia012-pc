#include "menu/agent29_types.h"

/* ScreenE75C4: polls the prompt; on 1 confirms (func_088AC8C8, clears the
   request result) and moves to state 3, on any other answer closes it and
   moves to state 0xA. */
void func_089E15F8(ScreenE75C4 *self)
{
    int result = func_089E34F8(self, &self->work->layout, self->work->handles, self->work->texts);
    if (result != 0) {
        if (result == 1) {
            func_088AC8C8(D_089D712C, 1);
            func_089E3128(self, 1, &self->work->layout, self->work->handles, self->work->texts);
            D_09B6D830.result = 0;
            self->work->state = 3;
        } else {
            func_089E3710(self, &self->work->layout, self->work->handles, self->work->texts);
            func_088D9CD8(self->work->handles[0x31]);
            self->work->state = 0xA;
        }
    }
}
