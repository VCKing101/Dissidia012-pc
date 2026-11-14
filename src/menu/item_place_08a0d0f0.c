#include "menu/agent29_types.h"

/* Screen11510: with page flag 8, places the cursor model (0x69) on the
   frame of item +0x109C of model 0x68 and, unless that item is locked
   (bit of +0x1144), its caption and shadow texts (0x4C and 0x52 + item)
   on the two frames of the item's model (0x6A + item). */
extern const unsigned short D_08A11E2A[];

void func_08A0D0F0(Screen11510 *self)
{
    float matrix[16];
    unsigned int color;
    int item;

    if (WORK_INT(self->work, 0xFD8) & 8) {
        item = WORK_INT(self->work, 0x109C);
        func_088DA0A8(self->work->handles[0x68], D_08A11E2A[item], matrix, &color);
        func_088D9B84(self->work->handles[0x69], matrix);
        func_088D9AF4(self->work->handles[0x69], &color);
        if ((WORK_BYTE(self->work, 0x1144) & (1 << item)) == 0) {
            func_088DA0A8(self->work->handles[0x6A + item], 1, matrix, &color);
            func_0887B778(self->work->texts[0x4C + item], matrix);
            func_0887B828(self->work->texts[0x4C + item], &color);
            func_088DA0A8(self->work->handles[0x6A + item], 0, matrix, &color);
            func_0887B778(self->work->texts[0x52 + item], matrix);
            func_0887B828(self->work->texts[0x52 + item], &color);
        }
    }
}
