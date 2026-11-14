#include "menu/auto_region3.h"

/* ScreenA690: hides handle 0x21 and redraws. */
void func_08AE4F0C(ScreenA690 *self)
{
    func_088D9CFC(self->work->handles[0x21]);
    func_08AE8758(self);
    func_08AE60D8(self);
}
