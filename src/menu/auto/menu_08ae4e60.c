#include "menu/auto_region3.h"

/* ScreenA690: plays motion 6 on handle 0x21 and redraws. */
void func_08AE4E60(ScreenA690 *self)
{
    func_088D9F5C(self->work->handles[0x21], 6, 1);
    func_08AE4EBC(self);
    func_08AE8758(self);
    func_08AE60D8(self);
    func_08AE7A8C(self);
}
