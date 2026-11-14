#include "menu/agent29_types.h"

/* Screen11510: places item `item`'s caption (text 0x5B + item) on the
   frame of its model (0x73 + item) for `state` (frame D_08A11D0A[state]);
   in state 1 also shows its highlight text (0x5E + item) on frame
   D_08A11D0A[2], otherwise hides it. */
extern const unsigned short D_08A11D0A[];

void func_08A0E110(Screen11510 *self, int state, int item)
{
    float matrix[16];
    unsigned int color;

    func_088DA0A8(self->work->handles[0x73 + item], D_08A11D0A[state], matrix, &color);
    func_0887B778(self->work->texts[0x5B + item], matrix);
    func_0887B828(self->work->texts[0x5B + item], &color);
    if (state == 1) {
        func_0887B924(self->work->texts[0x5E + item]);
        func_088DA0A8(self->work->handles[0x73 + item], D_08A11D0A[2], matrix, &color);
        func_0887B778(self->work->texts[0x5E + item], matrix);
        func_0887B828(self->work->texts[0x5E + item], &color);
    } else {
        func_0887B944(self->work->texts[0x5E + item]);
    }
}
