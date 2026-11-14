#include "menu/auto_region3.h"

/* ScreenA690: plays motions on handles 0x66 and 0x67 and shows handle 0x1F. */
void func_08AE656C(ScreenA690 *self)
{
    func_088D9F5C(self->work->handles[0x66], 2, 1);
    func_088D9F5C(self->work->handles[0x67], 5, 1);
    func_088D9CD8(self->work->handles[0x1F]);
}
