#include "menu/auto_region3.h"

/* ScreenDB30: plays motion 0x34 on handle 0x3C and shows handle 0x23. */
void func_08ACCB88(ScreenDB30 *self)
{
    func_088D9F5C(self->work->handles[0x3C], 0x34, 1);
    func_088D9CD8(self->work->handles[0x23]);
}
