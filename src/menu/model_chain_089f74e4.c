#include "menu/agent29_types.h"

/* Screen0B6A0: chains models onto each other's frames: each model is
   placed on a frame of the one before (the first frame from a table
   indexed by the current choice). */
extern const unsigned short D_08A0B410[];
extern const unsigned short D_08A0B328[];
extern const short D_08A09A4A[];
extern void func_088D9AF4(void *model, unsigned int *color);

/* With page flag 2: the count marks (0xEA, 0xEE, 0xF2) under the
   choice +0x31DC on model 0xE6. */

/* With page flag 4: the cursor models (0x7F, 0x80, 0x81) on the frame of
   the entry +0x2730 of model 0x13, then the cursor script. */
void func_089F74E4(Screen0B6A0 *self)
{
    float matrix[16];
    unsigned int color;

    if (self->work->flags & 4) {
        func_088DA0A8(self->work->handles[0x13], D_08A0B328[WORK_BYTE(self->work, 0x2730)], matrix, &color);
        func_088D9B84(self->work->handles[0x7F], matrix);
        func_088D9AF4(self->work->handles[0x7F], &color);
        func_088DA0A8(self->work->handles[0x7F], 0, matrix, &color);
        func_088D9B84(self->work->handles[0x80], matrix);
        func_088D9AF4(self->work->handles[0x80], &color);
        func_088D9B84(self->work->handles[0x81], matrix);
        func_088D9AF4(self->work->handles[0x81], &color);
        func_088D3F00(&self->work->layout, self->work->handles, D_08A09A4A);
    }
}
