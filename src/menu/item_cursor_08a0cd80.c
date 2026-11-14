#include "menu/agent29_types.h"

/* Screen11510: plays the opening script, places the cursor model (0x68)
   on the frame of the current item (+0x1070 less the first shown item
   +0x107C, through the frame table D_08A11E24), plays the item scripts
   and, with flag 1, refreshes the list. */
extern const short D_08A11B2E[];
extern const short D_08A11B3E[];
extern const unsigned short D_08A11C2E[];
extern const unsigned short D_08A11E24[];
extern void func_088D9AF4(void *model, unsigned int *color);
extern void func_08A0DFB4(Screen11510 *self);

void func_08A0CD80(Screen11510 *self)
{
    float matrix[16];
    unsigned int color;

    func_088D3F00(&self->work->layout, self->work->handles, D_08A11B2E);
    func_088DA0A8(self->work->handles[D_08A11E24[WORK_INT(self->work, 0x1070) - WORK_INT(self->work, 0x107C)]], 2,
                  matrix, &color);
    func_088D9B84(self->work->handles[0x68], matrix);
    func_088D9AF4(self->work->handles[0x68], &color);
    func_088D3F00(&self->work->layout, self->work->handles, D_08A11B3E);
    func_088D44EC(self->work->handles, self->work->texts, D_08A11C2E);
    if (self->work->flags & 1) {
        func_08A0DFB4(self);
    }
}
