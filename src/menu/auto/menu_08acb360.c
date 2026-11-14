#include "menu/auto_region3.h"

/* ScreenDB30: plays motion 0x1F on handle 0x21 and redraws. */
void func_08ACB360(ScreenDB30 *self)
{
    func_088D9F5C(self->work->handles[0x21], 0x1F, 1);
    func_08ACB474(self);
    func_08ACF548(self);
}
