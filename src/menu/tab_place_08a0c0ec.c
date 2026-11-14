#include "menu/agent29_types.h"

/* Screen11510: places the cursor model (0x2C) on the selected tab's frame
   (D_08A11DB0), and the selected tab's icon (0x40+) and texts (0x1B+,
   0x2B+) on the frames of its tab model (0x30+). */
extern int func_08A0BEEC(Screen11510 *self);
extern void func_088D9AF4(void *model, unsigned int *color);
extern const unsigned short D_08A11DB0[];

void func_08A0C0EC(Screen11510 *self)
{
    float matrix[16];
    unsigned int color;
    int tab = func_08A0BEEC(self);

    func_088DA0A8(self->work->handles[0x2D], D_08A11DB0[tab], matrix, &color);
    func_088D9B84(self->work->handles[0x2C], matrix);
    func_088D9AF4(self->work->handles[0x2C], &color);
    func_088DA0A8(self->work->handles[tab + 0x30], 2, matrix, &color);
    func_088D9B84(self->work->handles[tab + 0x40], matrix);
    func_088D9AF4(self->work->handles[tab + 0x40], &color);
    func_088DA0A8(self->work->handles[tab + 0x30], 1, matrix, &color);
    func_0887B778(self->work->texts[tab + 0x1B], matrix);
    func_0887B828(self->work->texts[tab + 0x1B], &color);
    func_088DA0A8(self->work->handles[tab + 0x30], 0, matrix, &color);
    func_0887B778(self->work->texts[tab + 0x2B], matrix);
    func_0887B828(self->work->texts[tab + 0x2B], &color);
}
