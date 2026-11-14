#include "menu/auto_region3.h"

/* ScreenA690: plays motion 6 on handle 0x20 and redraws. */
void func_08AE4D70(ScreenA690 *self)
{
    func_088D9F5C(self->work->handles[0x20], 6, 1);
    func_08AE4DD0(self);
    func_08AE5228(self, 1);
    func_08AE60D8(self);
    func_08AE7A8C(self);
}
