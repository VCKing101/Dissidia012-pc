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
void func_089F70F0(Screen0B6A0 *self)
{
    float matrix[16];
    unsigned int color;

    if (self->work->flags & 2) {
        func_088DA0A8(self->work->handles[0xE6], D_08A0B410[WORK_BYTE(self->work, 0x31DC)], matrix, &color);
        func_088D9B84(self->work->handles[0xEA], matrix);
        func_088D9AF4(self->work->handles[0xEA], &color);
        func_088DA0A8(self->work->handles[0xEA], 0, matrix, &color);
        func_088D9B84(self->work->handles[0xEE], matrix);
        func_088D9AF4(self->work->handles[0xEE], &color);
        func_088DA0A8(self->work->handles[0xEE], 0, matrix, &color);
        func_088D9B84(self->work->handles[0xF2], matrix);
        func_088D9AF4(self->work->handles[0xF2], &color);
    }
}

/* With page flag 4: the cursor models (0x7F, 0x80, 0x81) on the frame of
   the entry +0x2730 of model 0x13, then the cursor script. */
