#include "menu/auto_region3.h"

/* Screen850: resets its rows and hides handles 0x27, 0x28, 0x15 and 0xE. */
void func_08AE283C(Screen850 *self)
{
    func_08AE12AC(self, 1, 1, 0);
    func_08AE1748(self, 1, 0);
    func_08AE1BA8(self, -1);
    func_088D9CFC(self->work->handles[0x27]);
    func_088D9CFC(self->work->handles[0x28]);
    func_08AE2018(self, 3);
    func_088D9CFC(self->work->handles[0x15]);
    func_088D9CFC(self->work->handles[0xE]);
}
