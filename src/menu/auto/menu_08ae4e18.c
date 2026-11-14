#include "menu/auto_region3.h"

/* ScreenA690: hides handle 0x20 and redraws. */
void func_08AE4E18(ScreenA690 *self)
{
    func_088D9CFC(self->work->handles[0x20]);
    func_08AE5228(self, 1);
    func_08AE60D8(self);
}
