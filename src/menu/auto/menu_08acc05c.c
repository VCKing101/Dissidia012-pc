#include "menu/auto_region3.h"

/* ScreenDB30: hides handle 0x23. */
void func_08ACC05C(ScreenDB30 *self)
{
    func_088D9CFC(self->work->handles[0x23]);
    func_08AD40C0(self);
}
