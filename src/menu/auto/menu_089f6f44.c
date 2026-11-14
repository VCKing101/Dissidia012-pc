#include "menu/auto_region1.h"

/* Screen0B6A0: hides handles 0xEA, 0xEE and 0xF2. */
void func_089F6F44(Screen0B6A0 *self)
{
    func_088D9CFC(self->work->handles[0xEA]);
    func_088D9CFC(self->work->handles[0xEE]);
    func_088D9CFC(self->work->handles[0xF2]);
}
