#include "menu/agent29_types.h"

/* Screen181E0: frees the text pair of line `slot` (texts 0x60 + 2 * slot
   and its shadow) and, when `show` is 1, makes it again from message
   D_08A188E0[kind] of the root's messages. */
extern const unsigned short D_08A188E0[];

void func_08A16E40(Screen181E0 *self, unsigned char show, int slot, int kind)
{
    void *messages = D_08A23FF0->work->messages;

    if (self->work->texts[0x60 + slot * 2] != 0) {
        func_0887BA5C(self->work->texts[0x60 + slot * 2]);
        self->work->texts[0x60 + slot * 2] = 0;
    }
    if (self->work->texts[0x61 + slot * 2] != 0) {
        func_0887BA5C(self->work->texts[0x61 + slot * 2]);
        self->work->texts[0x61 + slot * 2] = 0;
    }
    if (show == 1) {
        self->work->texts[0x60 + slot * 2] = func_0887B3EC(messages, 8, 0, D_08A188E0[kind], 0, 0);
        self->work->texts[0x61 + slot * 2] = func_0887E534(self->work->texts[0x60 + slot * 2]);
    }
}
