#include "menu/auto_region2.h"

/* Panel61C40: plays its leave motions, hides handles 3..5, state 7. */
void func_08A60A34(Panel61C40 *self)
{
    func_088D9F5C(self->handles[0], 1, 1);
    func_088D9F5C(self->handles[1], 0x24, 1);
    func_088D9CFC(self->handles[3]);
    func_088D9CFC(self->handles[4]);
    func_088D9CFC(self->handles[5]);
    func_08A61618(self);
    self->state = 7;
}
